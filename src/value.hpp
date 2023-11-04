#pragma once

#include "runtime_error.hpp"
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

class BasicValue;
class IntegerValue;
class StringValue;
class BoolValue;

using Value = std::unique_ptr<BasicValue>;

class InvalidValueCast : public std::exception {
private:
    std::string m_message;

public:
    explicit InvalidValueCast(std::string_view const target_type)
        : m_message{ std::format("unable to cast value to target type '{}'", target_type) } { }

    [[nodiscard]] char const* what() const noexcept override {
        return m_message.c_str();
    }
};

class BasicValue {
protected:
    BasicValue() = default;

public:
    virtual ~BasicValue() = default;

    [[nodiscard]] virtual bool is_integer_value() const {
        return false;
    }

    [[nodiscard]] virtual IntegerValue const& as_integer_value() const {
        throw InvalidValueCast{ "IntegerValue" };
    }

    [[nodiscard]] virtual bool is_string_value() const {
        return false;
    }

    [[nodiscard]] virtual StringValue const& as_string_value() const {
        throw InvalidValueCast{ "StringValue" };
    }

    [[nodiscard]] virtual bool is_bool_value() const {
        return false;
    }

    [[nodiscard]] virtual BoolValue const& as_bool_value() const {
        throw InvalidValueCast{ "BoolValue" };
    }

    [[nodiscard]] virtual std::string string_representation() const = 0;

    [[nodiscard]] virtual Value unary_plus() const {
        throw OperationNotSupportedByType{ "unary plus", type_name() };
    }

    [[nodiscard]] virtual Value unary_minus() const {
        throw OperationNotSupportedByType{ "unary minus", type_name() };
    }

    [[nodiscard]] virtual Value binary_plus(Value const& other) const {
        throw OperationNotSupportedByType{ "binary plus", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value binary_minus(Value const& other) const {
        throw OperationNotSupportedByType{ "binary minus", type_name(), other->type_name() };
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

    [[nodiscard]] virtual Value less_than(Value const& other) const {
        throw OperationNotSupportedByType{ "less_than", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value less_or_equals(Value const& other) const {
        throw OperationNotSupportedByType{ "less_or_equals", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value assign(Value other) const {
        throw OperationNotSupportedByType{ "assignment", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual char const* type_name() const = 0;

    [[nodiscard]] virtual Value clone() const = 0;
};

class IntegerValue : public BasicValue {
public:
    using ValueType = std::int32_t;

private:
    ValueType m_value;

public:
    explicit IntegerValue(ValueType value) : m_value{ value } { }

    [[nodiscard]] bool is_integer_value() const override {
        return true;
    }

    [[nodiscard]] IntegerValue const& as_integer_value() const override {
        return *this;
    }

    [[nodiscard]] ValueType value() const {
        return m_value;
    }

    [[nodiscard]] std::string string_representation() const override {
        return std::to_string(value());
    }

    [[nodiscard]] char const* type_name() const noexcept override {
        return "I32";
    }

    [[nodiscard]] Value unary_plus() const override {
        return std::make_unique<IntegerValue>(m_value);
    }

    [[nodiscard]] Value unary_minus() const override {
        return std::make_unique<IntegerValue>(-m_value);
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override;

    [[nodiscard]] Value binary_minus(Value const& other) const override;

    [[nodiscard]] Value clone() const override {
        return std::make_unique<IntegerValue>(m_value);
    }

    [[nodiscard]] Value equals(Value const& other) const override;

    [[nodiscard]] Value not_equals(Value const& other) const override;

    [[nodiscard]] Value greater_than(Value const& other) const override;

    [[nodiscard]] Value greater_or_equals(Value const& other) const override;

    [[nodiscard]] Value less_than(Value const& other) const override;

    [[nodiscard]] Value less_or_equals(Value const& other) const override;

    [[nodiscard]] Value assign(Value other) const override;
};

class StringValue : public BasicValue {
public:
    using ValueType = std::string;

private:
    ValueType m_value;

public:
    explicit StringValue(ValueType value) : m_value{ std::move(value) } { }

    [[nodiscard]] bool is_string_value() const override {
        return true;
    }

    [[nodiscard]] StringValue const& as_string_value() const override {
        return *this;
    }

    [[nodiscard]] ValueType const& value() const {
        return m_value;
    }

    [[nodiscard]] std::string string_representation() const override {
        return value();
    }

    [[nodiscard]] char const* type_name() const noexcept override {
        return "String";
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override {
        return std::make_unique<StringValue>(m_value + other->string_representation());
    }

    [[nodiscard]] Value clone() const override {
        return std::make_unique<StringValue>(m_value);
    }
};

class BoolValue : public BasicValue {
public:
    using ValueType = bool;

private:
    ValueType m_value;

public:
    explicit BoolValue(ValueType value) : m_value{ value } { }

    [[nodiscard]] std::string string_representation() const override {
        if (m_value) {
            return "true";
        } else {
            return "false";
        }
    }

    [[nodiscard]] char const* type_name() const override {
        return "Bool";
    }

    [[nodiscard]] Value clone() const override {
        return std::make_unique<BoolValue>(m_value);
    }

    [[nodiscard]] bool value() const {
        return m_value;
    }

    [[nodiscard]] bool is_bool_value() const override {
        return true;
    }

    [[nodiscard]] BoolValue const& as_bool_value() const override {
        return *this;
    }
};

[[nodiscard]] inline Value make_integer_value(IntegerValue::ValueType const value) {
    return std::make_unique<IntegerValue>(value);
}

[[nodiscard]] inline Value make_string_value(StringValue::ValueType value) {
    return std::make_unique<StringValue>(std::move(value));
}

[[nodiscard]] inline Value make_bool_value(BoolValue::ValueType const value) {
    return std::make_unique<BoolValue>(value);
}
