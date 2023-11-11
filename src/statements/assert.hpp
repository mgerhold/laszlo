#pragma once

#include "../values/bool.hpp"
#include "statement.hpp"

namespace statements {
    class Assert final : public Statement {
    private:
        std::unique_ptr<expressions::Expression> m_expression;

    public:
        explicit Assert(std::unique_ptr<expressions::Expression> expression) : m_expression{ std::move(expression) } { }

        void execute(ScopeStack& scope_stack) const override {
            auto const evaluated = m_expression->evaluate(scope_stack);
            if (not evaluated->is_bool_value()) {
                throw TypeMismatch{ m_expression->source_location(), types::make_bool(), evaluated->type() };
            }
            if (not evaluated->as_bool_value().value()) {
                throw FailedAssertion{ m_expression->source_location() };
            }
        }
    };
} // namespace statements
