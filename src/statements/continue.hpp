#pragma once

#include "../control_flow.hpp"
#include "statement.hpp"

namespace statements {
    class Continue final : public Statement {
    private:
        Token m_continue_token;

    public:
        explicit Continue(Token const& continue_token) : m_continue_token{ continue_token } { }

        void execute(ScopeStack& scope_stack) const override {
            throw BreakException{ m_continue_token };
        }
    };


} // namespace statements
