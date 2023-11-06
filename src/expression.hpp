#pragma once

#include "scope.hpp"
#include "token.hpp"
#include "values/array.hpp"
#include "values/bool.hpp"
#include "values/integer.hpp"
#include "values/string.hpp"
#include <variant>

class StringLiteral;
class IntegerLiteral;
class UnaryOperator;

class Expression {
protected:
    Expression() = default;

public:
    virtual ~Expression() = default;

    [[nodiscard]] virtual Value evaluate(ScopeStack& scope_stack) const = 0;

    [[nodiscard]] virtual SourceLocation source_location() const = 0;
};

class IntegerLiteral final : public Expression {
private:
    Token m_token;

public:
    explicit IntegerLiteral(Token token) : m_token{ token } { }

    [[nodiscard]] Value evaluate([[maybe_unused]] ScopeStack& scope_stack) const override {
        return Integer::make(m_token.parse_integer(), ValueCategory::Rvalue);
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return m_token.source_location;
    }
};

class StringLiteral final : public Expression {
private:
    Token m_token;

public:
    explicit StringLiteral(Token token) : m_token{ token } { }

    [[nodiscard]] Value evaluate([[maybe_unused]] ScopeStack& scope_stack) const override {
        return String::make(
                std::string{ m_token.lexeme().substr(1, m_token.lexeme().length() - 2) },
                ValueCategory::Rvalue
        );
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return m_token.source_location;
    }
};

class BoolLiteral final : public Expression {
private:
    Token m_token;

public:
    explicit BoolLiteral(Token token) : m_token{ token } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        if (m_token.lexeme() == "true") {
            return Bool::make(true, ValueCategory::Rvalue);
        } else if (m_token.lexeme() == "false") {
            return Bool::make(false, ValueCategory::Rvalue);
        } else {
            assert(false and "unreachable");
            return {};
        }
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return m_token.source_location;
    }
};

class ArrayLiteral final : public Expression {
private:
    Token m_opening_bracket;
    std::vector<std::unique_ptr<Expression>> m_values;
    Token m_closing_bracket;

public:
    ArrayLiteral(
            Token const opening_bracket,
            std::vector<std::unique_ptr<Expression>> values,
            Token const closing_bracket
    )
        : m_opening_bracket{ opening_bracket },
          m_values{ std::move(values) },
          m_closing_bracket{ closing_bracket } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        auto elements = std::vector<Value>{};
        elements.reserve(m_values.size());
        for (auto const& value : m_values) {
            auto evaluated = value->evaluate(scope_stack);
            if (evaluated->is_lvalue()) {
                evaluated = evaluated->clone();
            } else {
                evaluated->promote_to_lvalue();
            }
            elements.push_back(std::move(evaluated));
        }

        if (not elements.empty()) {
            for (std::size_t i = 1; i < elements.size(); ++i) {
                if (elements.at(i)->type_name() != elements.front()->type_name()) {
                    throw TypeMismatch{ m_values.at(i)->source_location(),
                                        elements.front()->type_name(),
                                        elements.at(i)->type_name() };
                }
            }
        }

        for ([[maybe_unused]] auto const& element : elements) {
            assert(element->is_lvalue());
        }

        return Array::make(std::move(elements), ValueCategory::Rvalue);
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return SourceLocation::from_range(m_opening_bracket.source_location, m_closing_bracket.source_location);
    }
};

class UnaryOperator final : public Expression {
private:
    Token m_operator_token;
    std::unique_ptr<Expression> m_operand;

public:
    UnaryOperator(Token operator_token, std::unique_ptr<Expression> operand)
        : m_operator_token{ operator_token },
          m_operand{ std::move(operand) } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        switch (m_operator_token.type) {
            case TokenType::Plus:
                return m_operand->evaluate(scope_stack)->unary_plus();
            case TokenType::Minus:
                return m_operand->evaluate(scope_stack)->unary_minus();
            default:
                assert(false and "unreachable");
                return {};
        }
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return SourceLocation::from_range(m_operator_token.source_location, m_operand->source_location());
    }
};

class BinaryOperator : public Expression {
public:
    enum class Kind {
        Plus,
        Minus,
        Equality,
        Inequality,
        LessThan,
        LessOrEqual,
        GreaterThan,
        GreaterOrEqual,
        And,
        Or,
        Mod,
        Multiply,
        Divide,
    };

private:
    std::unique_ptr<Expression> m_left;
    Kind m_kind;
    std::unique_ptr<Expression> m_right;

public:
    BinaryOperator(std::unique_ptr<Expression> left, Kind const kind, std::unique_ptr<Expression> right)
        : m_left{ std::move(left) },
          m_kind{ kind },
          m_right{ std::move(right) } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        switch (m_kind) {
            case Kind::Plus:
                return m_left->evaluate(scope_stack)->binary_plus(m_right->evaluate(scope_stack));
            case Kind::Minus:
                return m_left->evaluate(scope_stack)->binary_minus(m_right->evaluate(scope_stack));
            case Kind::Equality:
                return m_left->evaluate(scope_stack)->equals(m_right->evaluate(scope_stack));
            case Kind::Inequality:
                return m_left->evaluate(scope_stack)->not_equals(m_right->evaluate(scope_stack));
            case Kind::LessThan:
                return m_left->evaluate(scope_stack)->less_than(m_right->evaluate(scope_stack));
            case Kind::LessOrEqual:
                return m_left->evaluate(scope_stack)->less_or_equals(m_right->evaluate(scope_stack));
            case Kind::GreaterThan:
                return m_left->evaluate(scope_stack)->greater_than(m_right->evaluate(scope_stack));
            case Kind::GreaterOrEqual:
                return m_left->evaluate(scope_stack)->greater_or_equals(m_right->evaluate(scope_stack));
            case Kind::And:
                return m_left->evaluate(scope_stack)->logical_and(m_right->evaluate(scope_stack));
            case Kind::Or:
                return m_left->evaluate(scope_stack)->logical_or(m_right->evaluate(scope_stack));
            case Kind::Mod:
                return m_left->evaluate(scope_stack)->mod(m_right->evaluate(scope_stack));
            case Kind::Multiply:
                return m_left->evaluate(scope_stack)->multiply(m_right->evaluate(scope_stack));
            case Kind::Divide:
                return m_left->evaluate(scope_stack)->divide(m_right->evaluate(scope_stack));
            default:
                assert(false and "unreachable");
                return {};
        }
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return SourceLocation::from_range(m_left->source_location(), m_right->source_location());
    }
};

class Name : public Expression {
private:
    Token m_name;

public:
    explicit Name(Token name) : m_name{ name } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        auto variable = scope_stack.lookup(std::string{ m_name.lexeme() });
        if (variable == nullptr) {
            throw UndefinedReference{ m_name };
        }
        return *variable;
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return m_name.source_location;
    }
};

class Range : public Expression {
private:
    std::unique_ptr<Expression> m_start;
    bool m_end_is_inclusive;
    std::unique_ptr<Expression> m_end;

public:
    Range(std::unique_ptr<Expression> start, bool const end_is_inclusive, std::unique_ptr<Expression> end)
        : m_start{ std::move(start) },
          m_end_is_inclusive{ end_is_inclusive },
          m_end{ std::move(end) } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        return m_start->evaluate(scope_stack)->range(m_end->evaluate(scope_stack), m_end_is_inclusive);
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return SourceLocation::from_range(m_start->source_location(), m_end->source_location());
    }
};

class Subscript : public Expression {
private:
    std::unique_ptr<Expression> m_expression;
    std::unique_ptr<Expression> m_subscript;
    Token m_closing_bracket;

public:
    Subscript(
            std::unique_ptr<Expression> expression,
            std::unique_ptr<Expression> subscript,
            Token const closing_bracket
    )
        : m_expression{ std::move(expression) },
          m_subscript{ std::move(subscript) },
          m_closing_bracket{ closing_bracket } { }

    [[nodiscard]] Value evaluate(ScopeStack& scope_stack) const override {
        return m_expression->evaluate(scope_stack)->subscript(m_subscript->evaluate(scope_stack));
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return SourceLocation::from_range(m_expression->source_location(), m_closing_bracket.source_location);
    }
};
