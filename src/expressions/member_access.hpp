#pragma once

#include "expression.hpp"

namespace expressions {

    class MemberAccess : public Expression {
    private:
        std::unique_ptr<Expression> m_object;
        Token m_member;

    public:
        MemberAccess(std::unique_ptr<Expression> object, Token member)
            : m_object{ std::move(object) },
              m_member{ member } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return m_object->evaluate(scope_stack)->member_access(m_member);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_object->source_location(), m_member.source_location);
        }
    };

} // namespace expressions
