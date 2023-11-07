#pragma once

#include "expression.hpp"

namespace expressions {
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

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return m_expression->evaluate(scope_stack)->subscript(m_subscript->evaluate(scope_stack));
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_expression->source_location(), m_closing_bracket.source_location);
        }
    };
}