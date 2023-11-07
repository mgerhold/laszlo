#pragma once

#include "expression.hpp"
#include "../values/string.hpp"

namespace expressions {
    class StringLiteral final : public Expression {
    private:
        Token m_token;

    public:
        explicit StringLiteral(Token token) : m_token{ token } { }

        [[nodiscard]] values::Value evaluate([[maybe_unused]] ScopeStack& scope_stack) const override {
            return values::String::make(
                    std::string{ m_token.lexeme().substr(1, m_token.lexeme().length() - 2) },
                    values::ValueCategory::Rvalue
            );
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return m_token.source_location;
        }
    };
}