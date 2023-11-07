#pragma once

#include "expression.hpp"
#include "../values/integer.hpp"

namespace expressions {
    class IntegerLiteral final : public Expression {
    private:
        Token m_token;

    public:
        explicit IntegerLiteral(Token token) : m_token{ token } { }

        [[nodiscard]] values::Value evaluate([[maybe_unused]] ScopeStack& scope_stack) const override {
            return values::Integer::make(m_token.parse_integer(), values::ValueCategory::Rvalue);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return m_token.source_location;
        }
    };
}