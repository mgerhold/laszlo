#pragma once

#include "expression.hpp"

namespace expressions {

    class Call final : public Expression {
    private:
        std::unique_ptr<Expression> m_callee;
        std::vector<std::unique_ptr<Expression>> m_arguments;
        Token m_closing_parenthesis;

    public:
        explicit Call(
                std::unique_ptr<Expression> callee,
                std::vector<std::unique_ptr<Expression>> arguments,
                Token const closing_parenthesis
        )
            : m_callee{ std::move(callee) },
              m_arguments{ std::move(arguments) },
              m_closing_parenthesis{ closing_parenthesis } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return m_callee->evaluate(scope_stack)->call(scope_stack, m_arguments);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_callee->source_location(), m_closing_parenthesis.source_location);
        }
    };

} // namespace expressions
