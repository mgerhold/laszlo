#pragma once

#include "string.hpp"
#include "value.hpp"

class Bool : public BasicValue {
public:
    using ValueType = bool;

private:
    ValueType m_value;

public:
    explicit Bool(ValueType const value, ValueCategory const value_category)
        : BasicValue{ value_category },
          m_value{ value } { }

    [[nodiscard]] static Value make(ValueType const value, ValueCategory const value_category) {
        return std::make_shared<Bool>(value, value_category);
    }

    [[nodiscard]] bool is_bool_value() const override {
        return true;
    }

    [[nodiscard]] Bool const& as_bool_value() const override {
        return *this;
    }

    [[nodiscard]] std::string string_representation() const override {
        if (m_value) {
            return "true";
        } else {
            return "false";
        }
    }

    [[nodiscard]] std::string type_name() const override {
        return "Bool";
    }

    [[nodiscard]] Value clone() const override {
        return make(m_value, value_category());
    }

    [[nodiscard]] bool value() const {
        return m_value;
    }

    [[nodiscard]] Value logical_and(Value const& other) const override {
        if (not other->is_bool_value()) {
            return BasicValue::logical_and(other); // throws
        }
        return make(value() and other->as_bool_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value logical_or(Value const& other) const override {
        if (not other->is_bool_value()) {
            return BasicValue::logical_and(other); // throws
        }
        return make(value() or other->as_bool_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override {
        if (not other->is_string_value()) {
            return BasicValue::binary_plus(other); // throws
        }
        return String::make(string_representation() + other->string_representation(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value equals(Value const& other) const override {
        if (not other->is_bool_value()) {
            return BasicValue::equals(other); // throws
        }
        return make(value() == other->as_bool_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value not_equals(Value const& other) const override {
        if (not other->is_bool_value()) {
            return BasicValue::not_equals(other); // throws
        }
        return make(value() != other->as_bool_value().value(), ValueCategory::Rvalue);
    }

    void assign(Value const& other) override {
        if (not other->is_bool_value()) {
            BasicValue::assign(other); // throws
        }
        m_value = other->as_bool_value().value();
    }
};
