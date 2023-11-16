#include "function.hpp"

#include "../control_flow.hpp"
#include "../expressions/expression.hpp"
#include "../statements/statement.hpp"
#include "nothing.hpp"

namespace values {
    Function::Function(
            Token const name,
            std::vector<statements::FunctionParameter> parameters,
            types::Type return_type,
            statements::Statement const* const body,
            ValueCategory const value_category
    )
        : BasicValue{ value_category },
          m_name{ name },
          m_parameters{ std::move(parameters) },
          m_return_type{ std::move(return_type) },
          m_body{ body } { }

    [[nodiscard]] Value Function::make(
            Token const name,
            std::vector<statements::FunctionParameter> parameters,
            types::Type return_type,
            statements::Statement const* const body,
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
        auto const num_scopes = scope_stack.size();
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
                throw SymbolRedefinition{ parameter.name() };
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
        scope_stack.truncate(num_scopes);
        return return_value;
    }

    [[nodiscard]] std::string Function::string_representation() const {
        auto parameters = std::string{};
        parameters.reserve(m_parameters.size());
        if (not m_parameters.empty()) {
            parameters += std::format(
                    "{}: {}",
                    m_parameters.front().name().lexeme(),
                    m_parameters.front().type()->to_string()
            );
        }
        for (auto i = std::size_t{ 1 }; i < m_parameters.size(); ++i) {
            parameters +=
                    std::format(", {}: {}", m_parameters.at(i).name().lexeme(), m_parameters.at(i).type()->to_string());
        }
        return std::format("function {}({}) ~> {}", m_name.lexeme(), std::move(parameters), m_return_type->to_string());
    }

    [[nodiscard]] types::Type Function::type() const {
        auto parameter_types = std::vector<types::Type>{};
        parameter_types.reserve(m_parameters.size());
        for (auto const& parameter : m_parameters) {
            parameter_types.push_back(parameter.type());
        }
        return types::make_function(std::move(parameter_types), m_return_type);
    }

    [[nodiscard]] Value Function::clone() const {
        return make(m_name, m_parameters, m_return_type, m_body, value_category());
    }
} // namespace values
