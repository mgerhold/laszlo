#pragma once

#include "scope.hpp"
#include "token.hpp"
#include "value.hpp"
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
        auto stream = std::stringstream{};
        stream << m_token.lexeme();
        auto value = IntegerValue::ValueType{};
        stream >> value;
        assert(stream);
        return make_integer_value(value);
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
        return make_string_value(std::string{ m_token.lexeme().substr(1, m_token.lexeme().length() - 2) });
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
            return make_bool_value(true);
        } else if (m_token.lexeme() == "false") {
            return make_bool_value(false);
        } else {
            assert(false and "unreachable");
            return {};
        }
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return m_token.source_location;
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
        auto const first_offset = m_operator_token.source_location.byte_offset;
        auto const last_offset = m_operand->source_location().byte_offset + m_operand->source_location().num_bytes;
        auto const length = last_offset - first_offset;
        return SourceLocation{ m_operator_token.source_location.filename,
                               m_operator_token.source_location.source,
                               first_offset,
                               length };
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
            default:
                assert(false and "unreachable");
                return {};
        }
    }

    [[nodiscard]] SourceLocation source_location() const override {
        auto const first_offset = m_left->source_location().byte_offset;
        auto const last_offset = m_right->source_location().byte_offset + m_right->source_location().num_bytes;
        auto const length = last_offset - first_offset;
        return SourceLocation{ m_left->source_location().filename,
                               m_left->source_location().source,
                               first_offset,
                               length };
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
        return (*variable)->clone();
    }

    [[nodiscard]] SourceLocation source_location() const override {
        return m_name.source_location;
    }
};
