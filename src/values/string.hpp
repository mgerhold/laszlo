#pragma once

#include "value.hpp"

class String : public BasicValue {
public:
    using ValueType = std::string;

private:
    ValueType m_value;

public:
    explicit String(ValueType value, ValueCategory const value_category)
        : BasicValue{ value_category },
          m_value{ std::move(value) } { }

    [[nodiscard]] static Value make(ValueType value, ValueCategory const value_category) {
        return std::make_shared<String>(std::move(value), value_category);
    }

    [[nodiscard]] bool is_string_value() const override {
        return true;
    }

    [[nodiscard]] String const& as_string_value() const override {
        return *this;
    }

    [[nodiscard]] ValueType const& value() const {
        return m_value;
    }

    [[nodiscard]] std::string string_representation() const override {
        return value();
    }

    [[nodiscard]] std::string type_name() const noexcept override {
        return "String";
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override {
        return make(m_value + other->string_representation(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value clone() const override {
        return make(m_value, value_category());
    }

    [[nodiscard]] Value multiply(Value const& other) const override {
        if (other->is_integer_value()) {
            return other->as_rvalue()->multiply(clone());
        }
        return BasicValue::multiply(other); // throws
    }

    void assign(Value const& other) override {
        if (not other->is_string_value()) {
            BasicValue::assign(other); // throws
        }
        m_value = other->as_string_value().m_value;
    }
};