#pragma once

#include "control_flow.hpp"
#include "expression.hpp"
#include "scope.hpp"
#include "values/iterator.hpp"
#include <list>

class Statement {
protected:
    Statement() = default;

public:
    virtual ~Statement() = default;

    virtual void execute(ScopeStack& scope_stack) const = 0;
};

using Statements = std::vector<std::unique_ptr<Statement>>;

class ExpressionStatement final : public Statement {
private:
    std::unique_ptr<Expression> m_expression;

public:
    explicit ExpressionStatement(std::unique_ptr<Expression> expression) : m_expression{ std::move(expression) } { }

    void execute(ScopeStack& scope_stack) const override {
        std::ignore = m_expression->evaluate(scope_stack);
    }
};

class Print final : public Statement {
private:
    std::unique_ptr<Expression> m_expression;

public:
    explicit Print(std::unique_ptr<Expression> expression) : m_expression{ std::move(expression) } { }

    void execute(ScopeStack& scope_stack) const override {
        if (m_expression == nullptr) {
            return;
        }
        std::cout << m_expression->evaluate(scope_stack)->string_representation();
    }
};

class Println final : public Statement {
private:
    std::unique_ptr<Expression> m_expression;

public:
    explicit Println(std::unique_ptr<Expression> expression) : m_expression{ std::move(expression) } { }

    void execute(ScopeStack& scope_stack) const override {
        if (m_expression == nullptr) {
            std::cout << '\n';
            return;
        }
        std::cout << m_expression->evaluate(scope_stack)->string_representation() << '\n';
    }
};

class If final : public Statement {
private:
    Token m_if_token;
    std::unique_ptr<Expression> m_condition;
    std::unique_ptr<Statement> m_then;
    std::unique_ptr<Statement> m_else;

public:
    If(Token if_token,
       std::unique_ptr<Expression> condition,
       std::unique_ptr<Statement> then,
       std::unique_ptr<Statement> else_)
        : m_if_token{ if_token },
          m_condition{ std::move(condition) },
          m_then{ std::move(then) },
          m_else{ std::move(else_) } { }

    void execute(ScopeStack& scope_stack) const override {
        auto const condition = m_condition->evaluate(scope_stack);
        if (not condition->is_bool_value()) {
            throw TypeMismatch{ m_if_token.source_location, "Bool", condition->type_name() };
        }
        auto const evaluated = condition->as_bool_value().value();
        if (evaluated) {
            m_then->execute(scope_stack);
        } else {
            m_else->execute(scope_stack);
        }
    }
};

class VariableDefinition final : public Statement {
private:
    Token m_name;
    std::unique_ptr<Expression> m_initializer;

public:
    VariableDefinition(Token const name, std::unique_ptr<Expression> initializer)
        : m_name{ name },
          m_initializer{ std::move(initializer) } { }

    void execute(ScopeStack& scope_stack) const override {
        if (m_name.lexeme() == "_") {
            return;
        }
        auto value = m_initializer->evaluate(scope_stack)->as_rvalue();
        value->promote_to_lvalue();
        auto const& [iterator, inserted] =
                scope_stack.top().insert({ std::string{ m_name.lexeme() }, std::move(value) });
        if (not inserted) {
            throw VariableRedefinition{ m_name };
        }
    }
};

class Block final : public Statement {
private:
    Statements m_statements;

public:
    explicit Block(Statements statements) : m_statements{ std::move(statements) } { }

    void execute(ScopeStack& scope_stack) const override {
        auto const num_scopes = scope_stack.size();
        scope_stack.push(Scope{});
        for (auto const& statement : m_statements) {
            statement->execute(scope_stack);
        }
        scope_stack.truncate(num_scopes);
    }
};

class Assert final : public Statement {
private:
    std::unique_ptr<Expression> m_expression;

public:
    explicit Assert(std::unique_ptr<Expression> expression) : m_expression{ std::move(expression) } { }

    void execute(ScopeStack& scope_stack) const override {
        auto const evaluated = m_expression->evaluate(scope_stack);
        if (not evaluated->is_bool_value()) {
            throw TypeMismatch{ m_expression->source_location(), "Bool", evaluated->type_name() };
        }
        if (not evaluated->as_bool_value().value()) {
            throw FailedAssertion{ m_expression->source_location() };
        }
    }
};

class Assignment final : public Statement {
private:
    std::unique_ptr<Expression> m_lvalue;
    std::unique_ptr<Expression> m_rvalue;

public:
    Assignment(std::unique_ptr<Expression> lvalue, std::unique_ptr<Expression> rvalue)
        : m_lvalue{ std::move(lvalue) },
          m_rvalue{ std::move(rvalue) } { }

    void execute(ScopeStack& scope_stack) const override {
        m_lvalue->evaluate(scope_stack)->assign(m_rvalue->evaluate(scope_stack));
    }
};

class While final : public Statement {
private:
    std::unique_ptr<Expression> m_condition;
    std::unique_ptr<Statement> m_body;

public:
    While(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
        : m_condition{ std::move(condition) },
          m_body{ std::move(body) } { }

    void execute(ScopeStack& scope_stack) const override {
        while (true) {
            auto const condition = m_condition->evaluate(scope_stack);
            if (not condition->is_bool_value()) {
                throw TypeMismatch{ m_condition->source_location(), "Bool", condition->type_name() };
            }
            if (not condition->as_bool_value().value()) {
                break;
            }
            try {
                m_body->execute(scope_stack);
            } catch (BreakException const&) {
                break;
            } catch (ContinueException const&) {
                // do nothing -> loop once more
            }
        }
    }
};

class Break : public Statement {
private:
    Token m_break_token;

public:
    explicit Break(Token const break_token) : m_break_token{ break_token } { }

    void execute(ScopeStack& scope_stack) const override {
        throw BreakException{ m_break_token };
    }
};

class Continue : public Statement {
private:
    Token m_continue_token;

public:
    explicit Continue(Token const continue_token) : m_continue_token{ continue_token } { }

    void execute(ScopeStack& scope_stack) const override {
        throw BreakException{ m_continue_token };
    }
};

class For : public Statement {
private:
    Token m_loop_variable;
    std::unique_ptr<Expression> m_iterator;
    std::unique_ptr<Statement> m_body;

public:
    For(Token const loop_variable, std::unique_ptr<Expression> iterator, std::unique_ptr<Statement> body)
        : m_loop_variable{ loop_variable },
          m_iterator{ std::move(iterator) },
          m_body{ std::move(body) } { }

    void execute(ScopeStack& scope_stack) const override {
        auto const num_scopes = scope_stack.size();

        auto current_iterator = m_iterator->evaluate(scope_stack);
        if (not current_iterator->is_iterator()) {
            throw TypeMismatch{ m_iterator->source_location(), "Iterator", current_iterator->type_name() };
        }

        while (true) {
            scope_stack.truncate(num_scopes);
            auto&& [iterator, value] = current_iterator->as_iterator().next();
            if (value->is_sentinel()) {
                break;
            }
            auto loop_scope = Scope{};
            if (m_loop_variable.lexeme() != "_") {
                loop_scope.insert({ std::string{ m_loop_variable.lexeme() }, std::move(value) });
            }
            scope_stack.push(std::move(loop_scope));
            current_iterator = std::move(iterator);
            try {
                m_body->execute(scope_stack);
            } catch (BreakException const&) {
                break;
            } catch (ContinueException const&) {
                // do nothing -> loop again
            }
        }
        scope_stack.truncate(num_scopes);
    }
};
