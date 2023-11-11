#pragma once

#include "../control_flow.hpp"
#include "statement.hpp"

namespace statements {
    class Return final : public Statement {
    private:
        Token m_return_token;
        std::optional<std::unique_ptr<expressions::Expression>> m_value;

    public:
        Return(Token const& return_token, std::optional<std::unique_ptr<expressions::Expression>> value)
            : m_return_token{ return_token },
              m_value{ std::move(value) } { }

        void execute(ScopeStack& scope_stack) const override {
            if (not m_value.has_value()) {
                throw ReturnException{ m_return_token };
            }
            throw ReturnException{ m_return_token, m_value.value()->evaluate(scope_stack) };
        }
    };
} // namespace statements
