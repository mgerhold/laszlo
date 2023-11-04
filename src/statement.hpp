#pragma once

#include "expression.hpp"
#include "scope.hpp"

class Statement {
protected:
    Statement() = default;

public:
    virtual ~Statement() = default;

    virtual void execute(ScopeStack& scope_stack) const = 0;
};

using Statements = std::vector<std::unique_ptr<Statement>>;

class Print final : public Statement {
private:
    std::unique_ptr<Expression> m_expression;

public:
    explicit Print(std::unique_ptr<Expression> expression) : m_expression{ std::move(expression) } { }

    void execute(ScopeStack& scope_stack) const override {
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
        auto const& [iterator, inserted] =
                scope_stack.top().insert({ std::string{ m_name.lexeme() }, m_initializer->evaluate(scope_stack) });
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
    Token m_lvalue;
    std::unique_ptr<Expression> m_value;

public:
    Assignment(Token lvalue, std::unique_ptr<Expression> value) : m_lvalue{ lvalue }, m_value{ std::move(value) } { }

    void execute(ScopeStack& scope_stack) const override {
        auto const variable = scope_stack.lookup(std::string{ m_lvalue.lexeme() });
        if (variable == nullptr) {
            throw UndefinedReference{ m_lvalue };
        }
        auto new_value = m_value->evaluate(scope_stack);
        *variable = (*variable)->assign(std::move(new_value));
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
            m_body->execute(scope_stack);
        }
    }
};
