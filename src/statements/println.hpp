#pragma once

#include "statement.hpp"

namespace statements {
    class Println final : public Statement {
    private:
        std::unique_ptr<expressions::Expression> m_expression;

    public:
        explicit Println(std::unique_ptr<expressions::Expression> expression)
            : m_expression{ std::move(expression) } { }

        void execute(ScopeStack& scope_stack) const override {
            if (m_expression == nullptr) {
                std::cout << '\n';
                return;
            }
            std::cout << m_expression->evaluate(scope_stack)->string_representation() << '\n';
        }
    };
} // namespace statements
