#pragma once

#include "../builtin_function_type.hpp"
#include "../expressions/expression.hpp"
#include "array.hpp"
#include "iterator.hpp"
#include "string.hpp"
#include "value.hpp"
#include <ranges>

namespace values {

    class BuiltinFunction final : public BasicValue {
    private:
        BuiltinFunctionType m_type;

    public:
        BuiltinFunction(BuiltinFunctionType const& type, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_type{ type } { }

        // clang-format off
        [[nodiscard]] static Value make(
                BuiltinFunctionType const& type,
                ValueCategory const value_category
        ) {
            return std::make_shared<BuiltinFunction>(type, value_category);
        }
        // clang-format on

        [[nodiscard]] std::string string_representation() const override {
            return std::format("BuiltinFunction({})", to_view(m_type));
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_builtin_function(m_type);
        }

        [[nodiscard]] Value clone() const override {
            return make(m_type, value_category());
        }

        // clang-format off
        [[nodiscard]] Value call(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const override { // clang-format on
            switch (m_type) {
                case BuiltinFunctionType::Split:
                    return split(scope_stack, arguments);
            }
            throw std::runtime_error{ "unreachable" };
        }

    private:
        // clang-format off
        [[nodiscard]] Value split(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const { // clang-format on
            if (arguments.size() != 3) {
                throw WrongNumberOfArguments{ to_view(m_type), 3, arguments.size() };
            }
            auto values = std::vector<Value>{};
            values.reserve(arguments.size());
            for (auto const& argument : arguments) {
                values.push_back(argument->evaluate(scope_stack));
            }

            if (values.front()->is_string_value()) {
                if (values.at(2)->type() != types::make_bool()) {
                    throw WrongArgumentType{ to_view(m_type), "discard_empty", values.at(2)->type() };
                }
                auto const discard_empty = values.at(2)->as_bool_value().value();
                if (values.at(1)->type() == types::make_char()) {
                    using std::ranges::views::filter;
                    using std::ranges::views::split;
                    using std::ranges::views::transform;
                    auto const separator = static_cast<char>(values.at(1)->as_char_value().value());
                    auto parts_view = values.front()->as_string().string_representation() | split(separator)
                                      | filter([discard_empty](auto const& part) {
                                            return not discard_empty or not std::string_view{ part }.empty();
                                        })
                                      | transform([](auto&& part) {
                                            return String::make(std::string_view{ part }, ValueCategory::Lvalue);
                                        });
                    auto parts = std::vector<Value>{ parts_view.begin(), parts_view.end() };
                    return Array::make(std::move(parts), ValueCategory::Rvalue);
                }
                throw WrongArgumentType{ to_view(m_type), "separator", values.at(1)->type() };
            }

            throw WrongArgumentType{ to_view(m_type), "splittable", values.front()->type() };
        }
    };

} // namespace values