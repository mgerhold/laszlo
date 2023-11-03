#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

class IntegerValue;
class StringValue;

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

    [[nodiscard]] virtual std::string string_representation() const = 0;
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
};

using Value = std::unique_ptr<BasicValue>;

[[nodiscard]] inline Value make_integer_value(IntegerValue::ValueType const value) {
    return std::make_unique<IntegerValue>(value);
}

[[nodiscard]] inline Value make_string_value(StringValue::ValueType value) {
    return std::make_unique<StringValue>(std::move(value));
}
