#pragma once

#include "../builtin_function_type.hpp"
#include "../expressions/expression.hpp"
#include "array.hpp"
#include "iterator.hpp"
#include "string.hpp"
#include "value.hpp"

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
                case BuiltinFunctionType::Join:
                    return join(scope_stack, arguments);
            }
            throw std::runtime_error{ "unreachable" };
        }

    private:
        // clang-format off
        [[nodiscard]] Value split(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const { // clang-format on
            if (arguments.size() < 2) {
                throw WrongNumberOfArguments{ to_view(m_type), 2, arguments.size() };
            }
            if (arguments.size() > 3) {
                throw WrongNumberOfArguments{ to_view(m_type), 3, arguments.size() };
            }
            auto values = std::vector<Value>{};
            values.reserve(arguments.size());
            for (auto const& argument : arguments) {
                values.push_back(argument->evaluate(scope_stack));
            }

            if (values.front()->is_string_value()) {
                if (values.size() > 2 and values.at(2)->type() != types::make_bool()) {
                    throw WrongArgumentType{ to_view(m_type), "discard_empty", values.at(2)->type() };
                }
                auto const discard_empty = values.size() > 2 and values.at(2)->as_bool_value().value();
                if (values.at(1)->type() == types::make_char()) {
                    using namespace std::string_literals;
                    auto const separator = static_cast<char>(values.at(1)->as_char_value().value());
                    auto const string = values.front()->as_string().string_representation();
                    auto current = ""s;
                    auto parts = std::vector<Value>{};
                    for (auto const c : string) {
                        if (c == separator) {
                            if (not discard_empty or not current.empty()) {
                                parts.push_back(
                                        String::make(std::string_view{ std::move(current) }, ValueCategory::Lvalue)
                                );
                            }
                            current.clear();
                            continue;
                        }
                        current += c;
                    }
                    if (not current.empty()) {
                        parts.push_back(String::make(std::string_view{ std::move(current) }, ValueCategory::Lvalue));
                    }
                    return Array::make(std::move(parts), ValueCategory::Rvalue);
                }
                throw WrongArgumentType{ to_view(m_type), "separator", values.at(1)->type() };
            }

            throw WrongArgumentType{ to_view(m_type), "splittable", values.front()->type() };
        }


        // clang-format off
        [[nodiscard]] Value join(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const { // clang-format on
            if (arguments.size() != 2) {
                throw WrongNumberOfArguments{ to_view(m_type), 2, arguments.size() };
            }
            auto values = std::vector<Value>{};
            values.reserve(arguments.size());
            for (auto const& argument : arguments) {
                values.push_back(argument->evaluate(scope_stack));
            }

            if (not values.at(1)->is_string_value() and not values.at(1)->is_char_value()) {
                throw WrongArgumentType{ to_view(m_type), "separator", values.at(1)->type() };
            }

            auto const separator = values.at(1)->string_representation();

            auto const iterator_value = values.front()->iterator();
            auto& iterator = iterator_value->as_iterator();
            auto elements = std::vector<std::string>{};
            while (true) {
                auto const next = iterator.next();
                if (next->is_sentinel()) {
                    break;
                }
                elements.push_back(next->string_representation());
            }

            auto joined = std::string{};
            for (auto i = std::size_t{ 0 }; i < elements.size(); ++i) {
                joined += elements.at(i);
                if (i < elements.size() - 1) {
                    joined += separator;
                }
            }

            return String::make(std::move(joined), ValueCategory::Rvalue);
        }
    };

} // namespace values
