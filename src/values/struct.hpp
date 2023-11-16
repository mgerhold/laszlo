#pragma once

#include "../statements/struct_definition.hpp"
#include "bool.hpp"
#include "value.hpp"
#include <unordered_map>

namespace values {
    class Struct final : public BasicValue {
    private:
        statements::StructDefinition const* m_definition;
        std::unordered_map<std::string, Value> m_members;

    public:
        Struct(statements::StructDefinition const* definition,
               std::unordered_map<std::string, Value> members,
               ValueCategory const value_category)
            : BasicValue{ value_category },
              m_definition{ definition },
              m_members{ std::move(members) } { }

        // clang-format off
        [[nodiscard]] static Value make(
                statements::StructDefinition const* definition,
                std::unordered_map<std::string, Value> members,
                ValueCategory const value_category
        ) { // clang-format on
            return std::make_shared<Struct>(definition, std::move(members), value_category);
        }

        [[nodiscard]] std::string string_representation() const override {
            auto result = std::format("struct {}(", m_definition->name().lexeme());
            for (std::size_t i = 0; i < m_definition->members().size(); ++i) {
                auto const& [token, type] = m_definition->members().at(i);
                result += std::format(
                        "{}: {}",
                        token.lexeme(),
                        m_members.at(std::string{ token.lexeme() })->string_representation()
                );
                if (i < m_definition->members().size() - 1) {
                    result += ", ";
                }
            }
            return result + ")";
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_struct_type(m_definition);
        }

        [[nodiscard]] Value clone() const override {
            auto members_copy = std::unordered_map<std::string, Value>{};
            members_copy.reserve(m_members.size());
            for (auto const& [key, value] : m_members) {
                members_copy.insert({ key, value->clone() });
            }
            return make(m_definition, std::move(members_copy), value_category());
        }


        [[nodiscard]] bool is_struct() const override {
            return true;
        }

        [[nodiscard]] Struct const& as_struct() const override {
            return *this;
        }

        [[nodiscard]] Value equals(Value const& other) const override {
            if (not other->is_struct()) {
                return BasicValue::equals(other); // throws
            }
            auto const& other_struct = other->as_struct();
            if (m_definition != other_struct.m_definition) {
                return BasicValue::equals(other); // throws
            }
            for (auto const& [name, value] : m_members) {
                if (not value->equals(other_struct.m_members.at(name))->as_bool_value().value()) {
                    return Bool::make(false, ValueCategory::Rvalue);
                }
            }
            return Bool::make(true, ValueCategory::Rvalue);
        }

        [[nodiscard]] Value member_access(Token const member) const override {
            auto const find_iterator = m_members.find(std::string{ member.lexeme() });
            auto const found = (find_iterator != m_members.end());
            if (not found) {
                return BasicValue::member_access(member); // throws
            }
            return find_iterator->second;
        }
    };
} // namespace values
