#pragma once

#include "statement.hpp"

namespace statements {
    class Block final : public Statement {
    private:
        Statements m_statements;

    public:
        explicit Block(Statements statements) : m_statements{ std::move(statements) } { }

        void execute(ScopeStack& scope_stack) const override {
            auto const num_scopes = scope_stack.size();
            scope_stack.push(Scope{});
            for (auto const& statement : m_statements) {
                statement->execute(scope_stack);
            }
            scope_stack.truncate(num_scopes);
        }
    };
} // namespace statements
