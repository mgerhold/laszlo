#pragma once

#include "../scope.hpp"
#include "nothing.hpp"
#include "value.hpp"

class Statement;
class FunctionParameter;

namespace values {

    class Function final : public BasicValue {
    private:
        Token m_name;
        std::vector<FunctionParameter> m_parameters;
        types::Type m_return_type;
        Statement const* m_body;

    public:
        Function(
                Token name,
                std::vector<FunctionParameter> parameters,
                types::Type return_type,
                Statement const* body,
                ValueCategory value_category
        );

        [[nodiscard]] static Value make(
                Token name,
                std::vector<FunctionParameter> parameters,
                types::Type return_type,
                Statement const* body,
                ValueCategory value_category
        );

        [[nodiscard]] std::string string_representation() const override {
            return "Function()";
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_function();
        }

        [[nodiscard]] Value clone() const override;

        [[nodiscard]] Value call(
                ScopeStack& scope_stack,
                std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const override;
    };

} // namespace values
