#pragma once

#include "../builtin_function_type.hpp"
#include "../expressions/expression.hpp"
#include "array.hpp"
#include "iterator.hpp"
#include "nothing.hpp"
#include "string.hpp"
#include "value.hpp"
#include <fstream>
#include <sstream>

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
                case BuiltinFunctionType::Delete:
                    return delete_(scope_stack, arguments);
                case BuiltinFunctionType::Write:
                    return write(scope_stack, arguments);
                case BuiltinFunctionType::Read:
                    return read(scope_stack, arguments);
                case BuiltinFunctionType::Trim:
                    return trim(scope_stack, arguments);
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

        // clang-format off
        [[nodiscard]] Value delete_(
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

            if (not values.front()->is_array() and not values.front()->is_string_value()) {
                throw WrongArgumentType{ to_view(m_type), "array", values.front()->type() };
            }

            if (not values.at(1)->is_integer_value()) {
                throw WrongArgumentType{ to_view(m_type), "index", values.at(1)->type() };
            }

            auto const container_size = [&] {
                if (values.front()->is_array()) {
                    return values.front()->as_array().value().size();
                }
                if (values.front()->is_string_value()) {
                    return values.front()->as_string().length();
                }
                assert(false and "unreachable");
                return std::size_t{ 0 };
            }();

            auto const index = values.at(1)->as_integer_value().value();
            if (index < 0 or static_cast<std::size_t>(index) >= container_size) {
                throw IndexOutOfBounds{ index, static_cast<int>(container_size) };
            }

            if (values.front()->is_array()) {
                values.front()->as_array().value().erase(std::next(values.front()->as_array().value().begin(), index));
            } else if (values.front()->is_string_value()) {
                values.front()->as_string().delete_(index);
            } else {
                assert(false and "unreachable");
            }

            return Nothing::make(ValueCategory::Rvalue);
        }

        // clang-format off
        [[nodiscard]] Value write(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const {
            // clang-format on
            if (arguments.size() != 2) {
                throw WrongNumberOfArguments{ to_view(m_type), 2, arguments.size() };
            }
            auto values = std::vector<Value>{};
            values.reserve(arguments.size());
            for (auto const& argument : arguments) {
                values.push_back(argument->evaluate(scope_stack));
            }

            if (values.at(0)->type() != types::make_string()) {
                throw WrongArgumentType{ to_view(m_type), "data", values.at(0)->type() };
            }

            if (values.at(1)->type() != types::make_string()) {
                throw WrongArgumentType{ to_view(m_type), "filename", values.at(1)->type() };
            }

            auto file = std::ofstream{ values.at(1)->as_string().string_representation() };
            if (not file) {
                // todo: dedicated exception type
                throw std::runtime_error{ "unable to open file for writing" };
            }

            file << values.at(0)->as_string().string_representation();
            if (not file) {
                // todo: dedicated exception type
                throw std::runtime_error{ "failed to write file" };
            }

            return Nothing::make(ValueCategory::Rvalue);
        }

        // clang-format off
        [[nodiscard]] Value read(
            ScopeStack& scope_stack,
            std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const { // clang-format on
            if (arguments.size() != 1) {
                throw WrongNumberOfArguments{ to_view(m_type), 2, arguments.size() };
            }
            auto values = std::vector<Value>{};
            values.reserve(arguments.size());
            for (auto const& argument : arguments) {
                values.push_back(argument->evaluate(scope_stack));
            }

            if (values.at(0)->type() != types::make_string()) {
                throw WrongArgumentType{ to_view(m_type), "filename", values.at(0)->type() };
            }

            auto file = std::ifstream{ values.at(0)->as_string().string_representation() };
            if (not file) {
                // todo: dedicated exception type
                throw std::runtime_error{ "unable to open file for reading" };
            }
            auto stream = std::ostringstream{};
            stream << file.rdbuf();
            if (not file) {
                // todo: dedicated exception type
                throw std::runtime_error{ "failed to read from file" };
            }

            return String::make(stream.str(), ValueCategory::Rvalue);
        }

        [[nodiscard]] Value trim(
                ScopeStack& scope_stack,
                std::vector<std::unique_ptr<expressions::Expression>> const& arguments
        ) const { // clang-format on
            if (arguments.size() != 1) {
                throw WrongNumberOfArguments{ to_view(m_type), 2, arguments.size() };
            }

            auto values = std::vector<Value>{};
            values.reserve(arguments.size());
            for (auto const& argument : arguments) {
                values.push_back(argument->evaluate(scope_stack));
            }

            if (values.at(0)->type() != types::make_string()) {
                throw WrongArgumentType{ to_view(m_type), "to_be_trimmed", values.at(0)->type() };
            }

            auto string = values.at(0)->as_string().string_representation();
            auto const left_find_iterator =
                    std::find_if(string.cbegin(), string.cend(), [](char const c) { return not std::isspace(c); });
            if (left_find_iterator == string.cend()) {
                // only whitespace found
                return String::make("", ValueCategory::Rvalue);
            }
            string.erase(string.cbegin(), left_find_iterator);

            auto const right_find_iterator =
                    std::find_if(string.crbegin(), string.crend(), [](char const c) { return not std::isspace(c); });
            if (right_find_iterator == string.crend()) {
                return String::make(std::move(string), ValueCategory::Rvalue);
            }
            string.erase(right_find_iterator.base(), string.cend());

            return String::make(std::move(string), ValueCategory::Rvalue);
        }
    };

} // namespace values
