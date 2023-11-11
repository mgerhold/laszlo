#pragma once

#include "../control_flow.hpp"
#include "../values/bool.hpp"
#include "statement.hpp"

namespace statements {
    class While final : public Statement {
    private:
        std::unique_ptr<expressions::Expression> m_condition;
        std::unique_ptr<Statement> m_body;

    public:
        While(std::unique_ptr<expressions::Expression> condition, std::unique_ptr<Statement> body)
            : m_condition{ std::move(condition) },
              m_body{ std::move(body) } { }

        void execute(ScopeStack& scope_stack) const override {
            while (true) {
                auto const condition = m_condition->evaluate(scope_stack);
                if (not condition->is_bool_value()) {
                    throw TypeMismatch{ m_condition->source_location(), types::make_bool(), condition->type() };
                }
                if (not condition->as_bool_value().value()) {
                    break;
                }
                try {
                    m_body->execute(scope_stack);
                } catch (BreakException const&) {
                    break;
                } catch (ContinueException const&) {
                    // do nothing -> loop once more
                }
            }
        }
    };
} // namespace statements
