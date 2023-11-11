#pragma once

#include "statement.hpp"

namespace statements {
    class ExpressionStatement final : public Statement {
    private:
        std::unique_ptr<expressions::Expression> m_expression;

    public:
        explicit ExpressionStatement(std::unique_ptr<expressions::Expression> expression)
            : m_expression{ std::move(expression) } { }

        void execute(ScopeStack& scope_stack) const override {
            std::ignore = m_expression->evaluate(scope_stack);
        }
    };
} // namespace statements
