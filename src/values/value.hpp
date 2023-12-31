#pragma once

#include "../runtime_error.hpp"
#include "../types.hpp"
#include <format>
#include <memory>
#include <stdexcept>

namespace values {
    class Struct;
}
class ScopeStack;

namespace expressions {
    class Expression;
}

namespace values {
    class BasicValue;

    using Value = std::shared_ptr<BasicValue>;

    class Integer;
    class Char;
    class String;
    class Bool;
    class Array;
    class Iterator;
    class StructType;

    enum class ValueCategory {
        Lvalue,
        Rvalue,
    };

    class BasicValue {
    private:
        ValueCategory m_value_category;

    protected:
        [[nodiscard]] ValueCategory value_category() const {
            return m_value_category;
        }

    public:
        explicit BasicValue(ValueCategory value_category) : m_value_category{ value_category } { }

        virtual ~BasicValue() = default;

        [[nodiscard]] bool is_lvalue() const {
            return m_value_category == ValueCategory::Lvalue;
        }

        [[nodiscard]] bool is_rvalue() const {
            return m_value_category == ValueCategory::Rvalue;
        }

        void promote_to_lvalue() {
            m_value_category = ValueCategory::Lvalue;
        }

        [[nodiscard]] Value as_rvalue() const {
            auto cloned = clone();
            cloned->m_value_category = ValueCategory::Rvalue;
            return cloned;
        }

        [[nodiscard]] virtual std::string string_representation() const = 0;

        [[nodiscard]] virtual types::Type type() const = 0;

        [[nodiscard]] virtual Value clone() const = 0;

        [[nodiscard]] virtual bool is_integer_value() const {
            return false;
        }

        [[nodiscard]] virtual Integer const& as_integer_value() const {
            throw InvalidValueCast{ "Integer" };
        }

        [[nodiscard]] virtual bool is_char_value() const {
            return false;
        }

        [[nodiscard]] virtual Char const& as_char_value() const {
            throw InvalidValueCast{ "Char" };
        }

        [[nodiscard]] virtual bool is_string_value() const {
            return false;
        }

        [[nodiscard]] virtual String const& as_string() const {
            throw InvalidValueCast{ "String" };
        }

        [[nodiscard]] virtual String& as_string() {
            throw InvalidValueCast{ "String" };
        }

        [[nodiscard]] virtual bool is_bool_value() const {
            return false;
        }

        [[nodiscard]] virtual Bool const& as_bool_value() const {
            throw InvalidValueCast{ "Bool" };
        }

        [[nodiscard]] virtual bool is_array() const {
            return false;
        }

        [[nodiscard]] virtual Array const& as_array() const {
            throw InvalidValueCast{ "Array" };
        }

        [[nodiscard]] virtual Array& as_array() {
            throw InvalidValueCast{ "Array" };
        }

        [[nodiscard]] virtual bool is_iterator() const {
            return false;
        }

        [[nodiscard]] virtual Iterator& as_iterator() {
            throw InvalidValueCast{ "Iterator" };
        }

        [[nodiscard]] virtual bool is_sentinel() const {
            return false;
        }

        [[nodiscard]] virtual bool is_struct_type() const {
            return false;
        }

        [[nodiscard]] virtual StructType const& as_struct_type() const {
            throw InvalidValueCast{ "StructType" };
        }

        [[nodiscard]] virtual bool is_struct() const {
            return false;
        }

        [[nodiscard]] virtual Struct const& as_struct() const {
            throw InvalidValueCast{ "Struct" };
        }

        [[nodiscard]] virtual Value unary_plus() const {
            throw OperationNotSupportedByType{ "unary_plus", type() };
        }

        [[nodiscard]] virtual Value unary_minus() const {
            throw OperationNotSupportedByType{ "unary_minus", type() };
        }

        [[nodiscard]] virtual Value binary_plus(Value const& other) const {
            throw OperationNotSupportedByType{ "binary_plus", type(), other->type() };
        }

        [[nodiscard]] virtual Value binary_minus(Value const& other) const {
            throw OperationNotSupportedByType{ "binary_minus", type(), other->type() };
        }

        [[nodiscard]] virtual Value multiply(Value const& other) const {
            throw OperationNotSupportedByType{ "multiply", type(), other->type() };
        }

        [[nodiscard]] virtual Value divide(Value const& other) const {
            throw OperationNotSupportedByType{ "multiply", type(), other->type() };
        }

        [[nodiscard]] virtual Value equals(Value const& other) const {
            throw OperationNotSupportedByType{ "equals", type(), other->type() };
        }

        [[nodiscard]] virtual Value not_equals(Value const& other) const;

        [[nodiscard]] virtual Value greater_than(Value const& other) const {
            throw OperationNotSupportedByType{ "greater_than", type(), other->type() };
        }

        [[nodiscard]] virtual Value greater_or_equals(Value const& other) const {
            throw OperationNotSupportedByType{ "greater_or_equals", type(), other->type() };
        }

        [[nodiscard]] virtual Value logical_and(Value const& other) const {
            throw OperationNotSupportedByType{ "logical_and", type(), other->type() };
        }

        [[nodiscard]] virtual Value logical_or(Value const& other) const {
            throw OperationNotSupportedByType{ "logical_or", type(), other->type() };
        }

        [[nodiscard]] virtual Value mod(Value const& other) const {
            throw OperationNotSupportedByType{ "mod", type(), other->type() };
        }

        [[nodiscard]] virtual Value less_than(Value const& other) const {
            throw OperationNotSupportedByType{ "less_than", type(), other->type() };
        }

        [[nodiscard]] virtual Value less_or_equals(Value const& other) const {
            throw OperationNotSupportedByType{ "less_or_equals", type(), other->type() };
        }

        [[nodiscard]] virtual Value range(Value const& other, bool end_is_inclusive) const {
            throw OperationNotSupportedByType{ "range", type(), other->type() };
        }

        [[nodiscard]] virtual Value subscript(Value const& index) const {
            throw OperationNotSupportedByType{ "subscript", type(), index->type() };
        }

        virtual void assign(Value const& other) {
            throw OperationNotSupportedByType{ "assignment", type(), other->type() };
        }

        [[nodiscard]] virtual Value iterator() {
            throw OperationNotSupportedByType{ "iterator", type() };
        }

        // clang-format off
        [[nodiscard]] virtual Value call(
                ScopeStack& scope_stack,
                [[maybe_unused]] std::vector<std::unique_ptr<expressions::Expression>> const&
        ) const;
        // clang-format on

        [[nodiscard]] virtual Value member_access(Token const member) const {
            throw NoSuchMember{ type(), member };
        }

        [[nodiscard]] virtual Value cast(types::Type const& target_type) const;
    };

} // namespace values
