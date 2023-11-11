#pragma once

#include "statement.hpp"

namespace statements {
    class FunctionParameter final {
        friend class FunctionDeclaration;

    private:
        Token m_name;
        types::Type m_type;

    public:
        FunctionParameter(Token const name, types::Type type) : m_name{ name }, m_type{ std::move(type) } { }

        [[nodiscard]] Token name() const {
            return m_name;
        }

        [[nodiscard]] types::Type const& type() const {
            return m_type;
        }
    };

    class FunctionDeclaration final : public Statement {
    private:
        Token m_name;
        std::vector<FunctionParameter> m_parameters;
        types::Type m_return_type;
        std::unique_ptr<Statement> m_body;

    public:
        FunctionDeclaration(
                Token const name,
                std::vector<FunctionParameter> parameters,
                types::Type return_type,
                std::unique_ptr<Statement> body
        )
            : m_name{ name },
              m_parameters{ std::move(parameters) },
              m_return_type{ std::move(return_type) },
              m_body{ std::move(body) } { }

        void execute(ScopeStack& scope_stack) const override;
    };
} // namespace statements
