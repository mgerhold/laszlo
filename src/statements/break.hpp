#pragma once

#include "../control_flow.hpp"
#include "statement.hpp"

namespace statements {
    class Break final : public Statement {
    private:
        Token m_break_token;

    public:
        explicit Break(Token const& break_token) : m_break_token{ break_token } { }

        void execute(ScopeStack& scope_stack) const override {
            throw BreakException{ m_break_token };
        }
    };
} // namespace statements
