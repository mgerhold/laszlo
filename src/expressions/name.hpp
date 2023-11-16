#pragma once

#include "expression.hpp"

namespace expressions {
    class Name final : public Expression {
    private:
        Token m_name;

    public:
        explicit Name(Token name) : m_name{ name } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            auto variable = scope_stack.lookup(std::string{ m_name.lexeme() });
            if (variable == nullptr) {
                throw UndefinedReference{ m_name };
            }
            return *variable;
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return m_name.source_location;
        }
    };
}