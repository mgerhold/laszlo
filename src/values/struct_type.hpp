#pragma once

#include "../statements/struct_definition.hpp"
#include "value.hpp"

namespace values {
    class StructType final : public BasicValue {
    public:
        using ValueType = statements::StructDefinition const*;

    private:
        ValueType m_definition;

    public:
        StructType(statements::StructDefinition const* const definition, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_definition{ definition } { }

        // clang-format off
        [[nodiscard]] static Value make(
            statements::StructDefinition const* const definition,
            ValueCategory const value_category
        ) { // clang-format on
            return std::make_shared<StructType>(definition, value_category);
        }

        [[nodiscard]] ValueType value() const {
            return m_definition;
        }

        [[nodiscard]] std::string string_representation() const override {
            return m_definition->to_string();
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_struct_type(m_definition);
        }

        [[nodiscard]] Value clone() const override {
            return make(m_definition, value_category());
        }

        [[nodiscard]] bool is_struct_type() const override {
            return true;
        }

        [[nodiscard]] StructType const& as_struct_type() const override {
            return *this;
        }
    };
} // namespace values
