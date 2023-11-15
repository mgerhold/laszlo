#pragma once

#include "builtin_function_type.hpp"
#include "source_location.hpp"
#include <format>
#include <memory>
#include <string>
#include <vector>

namespace types {
    class BasicType;

    using Type = std::shared_ptr<BasicType>;

    class Array;
    class Function;

    class BasicType {
    public:
        virtual ~BasicType() = default;

        [[nodiscard]] virtual std::string to_string() const = 0;

        [[nodiscard]] virtual bool equals(BasicType const& other) const = 0;

        [[nodiscard]] virtual bool can_be_created_from(Type const& other) const {
            return this->equals(*other);
        }

        [[nodiscard]] virtual bool is_array() const {
            return false;
        }

        [[nodiscard]] virtual Array const& as_array() const {
            throw std::runtime_error{ "unreachable" };
        }

        [[nodiscard]] virtual bool is_function() const {
            return false;
        }

        [[nodiscard]] virtual Function const& as_function() const {
            throw std::runtime_error{ "unreachable" };
        }
    };

    class I32 final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "I32";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<I32 const*>(&other) != nullptr;
        }
    };

    class Char final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Char";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Char const*>(&other) != nullptr;
        }
    };

    class Bool final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Bool";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Bool const*>(&other) != nullptr;
        }
    };

    class String final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "String";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<String const*>(&other) != nullptr;
        }
    };

    class Array final : public BasicType {
    private:
        Type m_contained_type;

    public:
        explicit Array(Type contained_type) : m_contained_type{ std::move(contained_type) } { }

        [[nodiscard]] std::string to_string() const override {
            return std::format("[{}]", m_contained_type->to_string());
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            if (auto const other_array = dynamic_cast<Array const*>(&other); other_array != nullptr) {
                return m_contained_type->equals(*other_array->m_contained_type);
            }
            return false;
        }

        [[nodiscard]] bool can_be_created_from(Type const& other) const override {
            return BasicType::can_be_created_from(other)
                   or (other->is_array() and m_contained_type->can_be_created_from(other->as_array().m_contained_type));
        }

        [[nodiscard]] bool is_array() const override {
            return true;
        }

        [[nodiscard]] Array const& as_array() const override {
            return *this;
        }
    };

    class Sentinel final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Sentinel";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Sentinel const*>(&other) != nullptr;
        }
    };

    class RangeIterator final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "RangeIterator";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<RangeIterator const*>(&other) != nullptr;
        }
    };

    class Range final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Range";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Range const*>(&other) != nullptr;
        }
    };

    class ArrayIterator final : public BasicType {
    private:
        Type m_array_type;

    public:
        explicit ArrayIterator(Type array_type) : m_array_type{ std::move(array_type) } { }

        [[nodiscard]] std::string to_string() const override {
            return std::format("ArrayIterator({})", m_array_type->to_string());
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            if (auto const other_iterator = dynamic_cast<ArrayIterator const*>(&other); other_iterator != nullptr) {
                return m_array_type->equals(*other_iterator->m_array_type);
            }
            return false;
        }
    };

    class StringIterator final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "StringIterator";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<StringIterator const*>(&other) != nullptr;
        }
    };

    class Unspecified final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "?";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Unspecified const*>(&other) != nullptr;
        }

        [[nodiscard]] bool can_be_created_from(Type const& other) const override {
            return true; // we can convert anything to "?"
        }
    };

    class Nothing final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Nothing";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Nothing const*>(&other) != nullptr;
        }
    };

    class BuiltinFunction final : public BasicType {
    private:
        BuiltinFunctionType m_type;

    public:
        explicit BuiltinFunction(BuiltinFunctionType const type) : m_type{ type } { }

        [[nodiscard]] std::string to_string() const override {
            return std::format("BuiltinFunction({})", to_view(m_type));
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<BuiltinFunction const*>(&other) != nullptr;
        }
    };

    class Function final : public BasicType {
    private:
        std::vector<Type> m_parameter_types;
        Type m_return_type;

    public:
        Function(std::vector<Type> parameter_types, Type return_type)
            : m_parameter_types{ std::move(parameter_types) },
              m_return_type{ std::move(return_type) } { }

        [[nodiscard]] std::string to_string() const override {
            auto parameters = std::string{};
            if (not m_parameter_types.empty()) {
                parameters += m_parameter_types.front()->to_string();
            }
            for (auto i = std::size_t{ 1 }; i < m_parameter_types.size(); ++i) {
                parameters += ", " + m_parameter_types.at(i)->to_string();
            }
            return std::format("Function({}) ~> {}", parameters, m_return_type->to_string());
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            if (not other.is_function()) {
                return false;
            }
            if (m_parameter_types.size() != other.as_function().m_parameter_types.size()) {
                return false;
            }
            for (auto i = std::size_t{ 0 }; i < m_parameter_types.size(); ++i) {
                if (not m_parameter_types.at(i)->equals(*other.as_function().m_parameter_types.at(i))) {
                    return false;
                }
            }
            return m_return_type->equals(*other.as_function().m_return_type);
        }

        [[nodiscard]] bool is_function() const override {
            return true;
        }

        [[nodiscard]] Function const& as_function() const override {
            return *this;
        }

        [[nodiscard]] bool can_be_created_from(Type const& other) const override {
            if (not other->is_function()) {
                return false;
            }
            auto const& other_function = other->as_function();
            if (m_parameter_types.size() != other_function.m_parameter_types.size()) {
                return false;
            }
            if (not m_return_type->can_be_created_from(other_function.m_return_type)) {
                return false;
            }
            for (std::size_t i = 0; i < m_parameter_types.size(); ++i) {
                if (not m_parameter_types.at(i)->can_be_created_from(other_function.m_parameter_types.at(i))) {
                    return false;
                }
            }
            return true;
        }
    };

    [[nodiscard]] inline bool operator==(Type const& lhs, Type const& rhs) {
        return lhs->equals(*rhs);
    }

    [[nodiscard]] inline Type make_bool() {
        return std::make_shared<Bool>();
    }

    [[nodiscard]] inline Type make_i32() {
        return std::make_shared<I32>();
    }

    [[nodiscard]] inline Type make_char() {
        return std::make_shared<Char>();
    }

    [[nodiscard]] inline Type make_string() {
        return std::make_shared<String>();
    }

    [[nodiscard]] inline Type make_sentinel() {
        return std::make_shared<Sentinel>();
    }

    [[nodiscard]] inline Type make_array(Type contained_type) {
        return std::make_shared<Array>(std::move(contained_type));
    }

    [[nodiscard]] inline Type make_array_iterator(Type array_type) {
        return std::make_shared<ArrayIterator>(std::move(array_type));
    }

    [[nodiscard]] inline Type make_string_iterator() {
        return std::make_shared<StringIterator>();
    }

    [[nodiscard]] inline Type make_range() {
        return std::make_shared<Range>();
    }

    [[nodiscard]] inline Type make_range_iterator() {
        return std::make_shared<RangeIterator>();
    }

    [[nodiscard]] inline Type make_function(std::vector<Type> parameter_types, Type return_type) {
        return std::make_shared<Function>(std::move(parameter_types), std::move(return_type));
    }

    [[nodiscard]] inline Type make_nothing() {
        return std::make_shared<Nothing>();
    }

    [[nodiscard]] inline Type make_unspecified() {
        return std::make_shared<Unspecified>();
    }

    [[nodiscard]] inline Type make_builtin_function(BuiltinFunctionType const type) {
        return std::make_shared<BuiltinFunction>(type);
    }
} // namespace types
