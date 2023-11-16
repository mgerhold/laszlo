#pragma once

#include "../values/nothing.hpp"
#include "../values/struct.hpp"
#include "expression.hpp"

namespace expressions {
    struct StructMemberInitializer final {
        Token name;
        std::unique_ptr<Expression> value;
    };

    class StructLiteral final : public Expression {
    private:
        Token m_name;
        std::vector<StructMemberInitializer> m_initializers;
        Token m_closing_curly_bracket_token;

    public:
        StructLiteral(
                Token const& name,
                std::vector<StructMemberInitializer> m_initializers,
                Token const& closing_curly_bracket_token
        )
            : m_name{ name },
              m_initializers{ std::move(m_initializers) },
              m_closing_curly_bracket_token{ closing_curly_bracket_token } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override;

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_name.source_location, m_closing_curly_bracket_token.source_location);
        }
    };
} // namespace expressions
