#pragma once

#include "../scope.hpp"
#include "..\statements\function_definition.hpp"
#include "value.hpp"

namespace statements {
    class Statement;
    class FunctionParameter;
} // namespace statements

namespace values {

    class Function final : public BasicValue {
    private:
        Token m_name;
        std::vector<statements::FunctionParameter> m_parameters;
        types::Type m_return_type;
        statements::Statement const* m_body;

    public:
        Function(
                Token name,
                std::vector<statements::FunctionParameter> parameters,
                types::Type return_type,
                statements::Statement const* body,
                ValueCategory value_category
        );

        [[nodiscard]] static Value make(
                Token name,
                std::vector<statements::FunctionParameter> parameters,
                types::Type return_type,
                statements::Statement const* body,
                ValueCategory value_category
        );

        [[nodiscard]] std::string string_representation() const override;

        [[nodiscard]] types::Type type() const override;

        [[nodiscard]] Value clone() const override;

        [[nodiscard]] Value call(
                ScopeStack& scope_stack,
                std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const override;
    };

} // namespace values
