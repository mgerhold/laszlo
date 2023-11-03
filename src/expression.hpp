#pragma once

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

    [[nodiscard]] virtual Value evaluate() const = 0;
};

class IntegerLiteral final : public Expression {
private:
    Token m_token;

public:
    explicit IntegerLiteral(Token token) : m_token{ token } { }

    [[nodiscard]] Token const& token() const {
        return m_token;
    }

    [[nodiscard]] Value evaluate() const override {
        auto stream = std::stringstream{};
        stream << m_token.lexeme();
        auto value = IntegerValue::ValueType{};
        stream >> value;
        assert(stream);
        return make_integer_value(value);
    }
};

class StringLiteral final : public Expression {
private:
    Token m_token;

public:
    explicit StringLiteral(Token token) : m_token{ token } { }

    [[nodiscard]] Token const& token() const {
        return m_token;
    }

    [[nodiscard]] Value evaluate() const override {
        return make_string_value(std::string{ m_token.lexeme().substr(1, m_token.lexeme().length() - 2) });
    }
};

class UnaryOperator final : public Expression {
public:
    enum class Kind {
        Plus,
        Minus,
    };

private:
    Kind m_kind;
    std::unique_ptr<Expression> m_operand;

public:
    template<typename T>
    UnaryOperator(Kind kind, T&& operand)
        : m_kind{ kind },
          m_operand{ std::make_unique<T>(std::forward<T>(operand)) } { }
};
