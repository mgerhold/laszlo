#pragma once

#include "expression.hpp"

namespace expressions {
    class UnaryOperator final : public Expression {
    private:
        Token m_operator_token;
        std::unique_ptr<Expression> m_operand;

    public:
        UnaryOperator(Token operator_token, std::unique_ptr<Expression> operand)
            : m_operator_token{ operator_token },
              m_operand{ std::move(operand) } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
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
}