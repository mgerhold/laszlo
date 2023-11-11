#pragma once

#include "../values/bool.hpp"
#include "statement.hpp"

namespace statements {
    class If final : public Statement {
    private:
        Token m_if_token;
        std::unique_ptr<expressions::Expression> m_condition;
        std::unique_ptr<Statement> m_then;
        std::unique_ptr<Statement> m_else;

    public:
        If(Token const& if_token,
           std::unique_ptr<expressions::Expression> condition,
           std::unique_ptr<Statement> then,
           std::unique_ptr<Statement> else_)
            : m_if_token{ if_token },
              m_condition{ std::move(condition) },
              m_then{ std::move(then) },
              m_else{ std::move(else_) } { }

        void execute(ScopeStack& scope_stack) const override {
            auto const condition = m_condition->evaluate(scope_stack);
            if (not condition->is_bool_value()) {
                throw TypeMismatch{ m_if_token.source_location, std::make_unique<types::Bool>(), condition->type() };
            }
            auto const evaluated = condition->as_bool_value().value();
            if (evaluated) {
                m_then->execute(scope_stack);
            } else {
                m_else->execute(scope_stack);
            }
        }
    };
} // namespace statements
