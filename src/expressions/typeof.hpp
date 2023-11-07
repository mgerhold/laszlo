#pragma once

#include "../values/string.hpp"
#include "expression.hpp"

namespace expressions {
    class TypeOf final : public Expression {
    private:
        Token m_typeof_token;
        std::unique_ptr<Expression> m_expression;
        Token m_closing_parenthesis_token;

    public:
        explicit TypeOf(
                Token const typeof_token,
                std::unique_ptr<Expression> expression,
                Token const closing_parenthesis_token
        )
            : m_typeof_token{ typeof_token },
              m_expression{ std::move(expression) },
              m_closing_parenthesis_token{ closing_parenthesis_token } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return values::String::make(
                    m_expression->evaluate(scope_stack)->type()->to_string(),
                    values::ValueCategory::Rvalue
            );
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(
                    m_typeof_token.source_location,
                    m_closing_parenthesis_token.source_location
            );
        }
    };
} // namespace expressions
