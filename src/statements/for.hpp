#pragma once

#include "../control_flow.hpp"
#include "../values/iterator.hpp"
#include "statement.hpp"

namespace statements {
    class For final : public Statement {
    private:
        Token m_loop_variable;
        std::unique_ptr<expressions::Expression> m_iterable;
        std::unique_ptr<Statement> m_body;

    public:
        For(Token const loop_variable,
            std::unique_ptr<expressions::Expression> iterable,
            std::unique_ptr<Statement> body)
            : m_loop_variable{ loop_variable },
              m_iterable{ std::move(iterable) },
              m_body{ std::move(body) } { }

        void execute(ScopeStack& scope_stack) const override {
            auto const num_scopes = scope_stack.size();

            auto iterator = m_iterable->evaluate(scope_stack)->iterator();
            assert(iterator->is_iterator());

            while (true) {
                scope_stack.truncate(num_scopes);
                auto value = iterator->as_iterator().next();
                if (value->is_sentinel()) {
                    break;
                }
                auto loop_scope = Scope{};
                if (m_loop_variable.lexeme() != "_") {
                    loop_scope.insert({ std::string{ m_loop_variable.lexeme() }, std::move(value) });
                }
                scope_stack.push(std::move(loop_scope));
                try {
                    m_body->execute(scope_stack);
                } catch (BreakException const&) {
                    break;
                } catch (ContinueException const&) {
                    // do nothing -> loop again
                }
            }
            scope_stack.truncate(num_scopes);
        }
    };
} // namespace statements
