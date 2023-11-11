#pragma once

#include "../values/char.hpp"
#include "expression.hpp"

namespace expressions {
    class CharLiteral final : public Expression {
    private:
        Token m_char_token;

    public:
        explicit CharLiteral(Token const& char_token) : m_char_token{ char_token } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return values::Char::make(
                    static_cast<values::Char::ValueType>(m_char_token.lexeme()[1]),
                    values::ValueCategory::Rvalue
            );
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return m_char_token.source_location;
        }
    };
} // namespace expressions
