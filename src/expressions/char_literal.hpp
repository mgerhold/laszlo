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
            auto const is_escape_sequence = (m_char_token.lexeme().length() == 4);
            assert((not is_escape_sequence and m_char_token.lexeme().length() == 3)
                   or (m_char_token.lexeme().length() == 4 and is_escape_sequence));
            if (is_escape_sequence) {
                auto c = '\0';
                switch (m_char_token.lexeme()[2]) {
                    case 'n':
                        c = '\n';
                        break;
                    case 'r':
                        c = '\r';
                        break;
                    case 't':
                        c = '\t';
                        break;
                    case 'v':
                        c = '\v';
                        break;
                    case '\\':
                        c = '\\';
                        break;
                    case '\'':
                        c = '\'';
                        break;
                    default:
                        assert(false and "unreachable");
                        break;
                }
                assert(c != '\0');
                return values::Char::make(static_cast<values::Char::ValueType>(c), values::ValueCategory::Rvalue);
            }
            assert(not is_escape_sequence);
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
