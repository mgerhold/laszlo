#pragma once

#include "statement.hpp"

namespace statements {
    class VariableDefinition final : public Statement {
    private:
        Token m_name;
        std::unique_ptr<expressions::Expression> m_initializer;

    public:
        VariableDefinition(Token const& name, std::unique_ptr<expressions::Expression> initializer)
            : m_name{ name },
              m_initializer{ std::move(initializer) } { }

        void execute(ScopeStack& scope_stack) const override {
            if (m_name.lexeme() == "_") {
                return;
            }
            auto value = m_initializer->evaluate(scope_stack)->as_rvalue();
            value->promote_to_lvalue();
            auto const& [iterator, inserted] =
                    scope_stack.top().insert({ std::string{ m_name.lexeme() }, std::move(value) });
            if (not inserted) {
                throw SymbolRedefinition{ m_name };
            }
        }
    };
} // namespace statements
