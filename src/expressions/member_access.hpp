#pragma once

#include "expression.hpp"

namespace expressions {

    class MemberAccess final : public Expression {
    private:
        std::unique_ptr<Expression> m_lhs;
        Token m_member;

    public:
        MemberAccess(std::unique_ptr<Expression> object, Token const& member)
            : m_lhs{ std::move(object) },
              m_member{ member } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return m_lhs->evaluate(scope_stack)->member_access(m_member);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_lhs->source_location(), m_member.source_location);
        }

        [[nodiscard]] std::unique_ptr<Expression> move_lhs_out() {
            return std::move(m_lhs);
        }

        [[nodiscard]] Token member_token() const {
            return m_member;
        }
    };

} // namespace expressions
