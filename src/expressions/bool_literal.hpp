#pragma once

#include "expression.hpp"
#include "../values/bool.hpp"

namespace expressions {
    class BoolLiteral final : public Expression {
    private:
        Token m_token;

    public:
        explicit BoolLiteral(Token token) : m_token{ token } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            if (m_token.lexeme() == "true") {
                return values::Bool::make(true, values::ValueCategory::Rvalue);
            } else if (m_token.lexeme() == "false") {
                return values::Bool::make(false, values::ValueCategory::Rvalue);
            } else {
                assert(false and "unreachable");
                return {};
            }
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return m_token.source_location;
        }
    };
}