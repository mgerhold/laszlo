#pragma once

#include "../runtime_error.hpp"
#include <format>
#include <memory>
#include <stdexcept>

namespace values {

    class BasicValue;

    using Value = std::shared_ptr<BasicValue>;

    class Integer;
    class String;
    class Bool;
    class Array;
    class Iterator;

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

        [[nodiscard]] virtual std::string type_name() const = 0;

        [[nodiscard]] virtual Value clone() const = 0;

        [[nodiscard]] virtual bool is_integer_value() const {
            return false;
        }

        [[nodiscard]] virtual Integer const& as_integer_value() const {
            throw InvalidValueCast{ "Integer" };
        }

        [[nodiscard]] virtual bool is_string_value() const {
            return false;
        }

        [[nodiscard]] virtual String const& as_string_value() const {
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

        [[nodiscard]] virtual bool is_iterator() const {
            return false;
        }

        [[nodiscard]] virtual Iterator& as_iterator() {
            throw InvalidValueCast{ "Iterator" };
        }

        [[nodiscard]] virtual bool is_sentinel() const {
            return false;
        }

        [[nodiscard]] virtual Value unary_plus() const {
            throw OperationNotSupportedByType{ "unary_plus", type_name() };
        }

        [[nodiscard]] virtual Value unary_minus() const {
            throw OperationNotSupportedByType{ "unary_minus", type_name() };
        }

        [[nodiscard]] virtual Value binary_plus(Value const& other) const {
            throw OperationNotSupportedByType{ "binary_plus", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value binary_minus(Value const& other) const {
            throw OperationNotSupportedByType{ "binary_minus", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value multiply(Value const& other) const {
            throw OperationNotSupportedByType{ "multiply", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value divide(Value const& other) const {
            throw OperationNotSupportedByType{ "multiply", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value equals(Value const& other) const {
            throw OperationNotSupportedByType{ "equals", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value not_equals(Value const& other) const {
            throw OperationNotSupportedByType{ "not_equals", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value greater_than(Value const& other) const {
            throw OperationNotSupportedByType{ "greater_than", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value greater_or_equals(Value const& other) const {
            throw OperationNotSupportedByType{ "greater_or_equals", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value logical_and(Value const& other) const {
            throw OperationNotSupportedByType{ "logical_and", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value logical_or(Value const& other) const {
            throw OperationNotSupportedByType{ "logical_or", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value mod(Value const& other) const {
            throw OperationNotSupportedByType{ "mod", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value less_than(Value const& other) const {
            throw OperationNotSupportedByType{ "less_than", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value less_or_equals(Value const& other) const {
            throw OperationNotSupportedByType{ "less_or_equals", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value range(Value const& other, bool end_is_inclusive) const {
            throw OperationNotSupportedByType{ "range", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value subscript(Value const& index) const {
            throw OperationNotSupportedByType{ "subscript", type_name(), index->type_name() };
        }

        virtual void assign(Value const& other) {
            throw OperationNotSupportedByType{ "assignment", type_name(), other->type_name() };
        }

        [[nodiscard]] virtual Value iterator() {
            throw OperationNotSupportedByType{ "iterator", type_name() };
        }
    };

} // namespace values
