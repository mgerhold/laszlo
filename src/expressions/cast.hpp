#pragma once

#include "expression.hpp"

namespace expressions {
    class Cast final : public Expression {
    private:
        std::unique_ptr<Expression> m_expression;
        types::Type m_target_type;

    public:
        Cast(std::unique_ptr<Expression> expression, types::Type target_type)
            : m_expression{ std::move(expression) },
              m_target_type{ std::move(target_type) } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return m_expression->evaluate(scope_stack)->cast(m_target_type);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return m_expression->source_location();
        }
    };
} // namespace expressions
