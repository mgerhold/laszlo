#include "function.hpp"
#include "../expressions/expression.hpp"
#include "../statement.hpp"

namespace values {
    Function::Function(
            Token const name,
            std::vector<FunctionParameter> parameters,
            types::Type return_type,
            Statement const* const body,
            ValueCategory const value_category
    )
        : BasicValue{ value_category },
          m_name{ name },
          m_parameters{ std::move(parameters) },
          m_return_type{ std::move(return_type) },
          m_body{ body } { }

    [[nodiscard]] Value Function::make(
            Token const name,
            std::vector<FunctionParameter> parameters,
            types::Type return_type,
            Statement const* const body,
            ValueCategory const value_category
    ) {
        return std::make_shared<Function>(name, std::move(parameters), std::move(return_type), body, value_category);
    }

    [[nodiscard]] Value Function::call(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
    ) const {
        if (m_parameters.size() != arguments.size()) {
            throw WrongNumberOfArguments{ m_name, m_parameters.size(), arguments.size() };
        }
        auto function_scope = Scope{};
        auto const num_args = arguments.size();
        for (std::size_t i = 0; i < num_args; ++i) {
            auto const& parameter = m_parameters.at(i);
            auto const& argument_expression = arguments.at(i);
            auto argument = argument_expression->evaluate(scope_stack);
            if (not parameter.type()->can_be_created_from(argument->type())) {
                throw WrongArgumentType{ parameter.name(), parameter.type(), argument->type() };
            }

            auto const& [iterator, inserted] =
                    function_scope.insert({ std::string{ parameter.name().lexeme() }, std::move(argument) });
            if (not inserted) {
                throw VariableRedefinition{ parameter.name() };
            }
        }

        scope_stack.push(std::move(function_scope));

        auto return_value = values::Nothing::make(ValueCategory::Rvalue);
        try {
            m_body->execute(scope_stack);
        } catch (ReturnException const& e) {
            auto returned_value = e.value();
            if (returned_value.has_value()) {
                return_value = std::move(returned_value.value());
            }
        }
        if (not m_return_type->can_be_created_from(return_value->type())) {
            throw ReturnTypeMismatch{ m_name.source_location, m_return_type, return_value->type() };
        }
        return return_value;
    }

    [[nodiscard]] Value Function::clone() const {
        return make(m_name, m_parameters, m_return_type, m_body, value_category());
    }
} // namespace values
