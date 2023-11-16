#pragma once

#include "statement.hpp"

namespace statements {
    struct StructMember final {
        Token m_name;
        types::Type m_type;
    };

    class StructDefinition final : public Statement {
    private:
        Token m_name;
        std::vector<StructMember> m_members;

    public:
        StructDefinition(Token const& name, std::vector<StructMember> members)
            : m_name{ name },
              m_members{ std::move(members) } { }

        void execute(ScopeStack& scope_stack) const override;

        [[nodiscard]] std::string to_string() const {
            auto result = std::format("struct {}(", m_name.lexeme());
            for (auto i = std::size_t{ 0 }; i < m_members.size(); ++i) {
                result += std::format(
                        "{}: {}",
                        m_members.at(i).m_name.lexeme(),
                        m_members.at(i).m_type->to_string()
                );
                if (i < m_members.size() - 1) {
                    result += ", ";
                }
            }
            return result + ")";
        }

        [[nodiscard]] Token name() const {
            return m_name;
        }

        [[nodiscard]] std::vector<StructMember> const& members() const {
            return m_members;
        }
    };
} // namespace statements
