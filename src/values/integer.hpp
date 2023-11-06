#pragma once

#include "string.hpp"
#include "value.hpp"
#include "bool.hpp"
#include <memory>
#include <string>

class Integer : public BasicValue {
public:
    using ValueType = std::int32_t;

private:
    ValueType m_value;

public:
    explicit Integer(ValueType value, ValueCategory value_category) : BasicValue{ value_category }, m_value{ value } { }

    [[nodiscard]] static Value make(ValueType value, ValueCategory value_category) {
        return std::make_shared<Integer>(value, value_category);
    }

    [[nodiscard]] bool is_integer_value() const override {
        return true;
    }

    [[nodiscard]] Integer const& as_integer_value() const override {
        return *this;
    }

    [[nodiscard]] ValueType value() const {
        return m_value;
    }

    [[nodiscard]] std::string string_representation() const override {
        return std::to_string(value());
    }

    [[nodiscard]] std::string type_name() const noexcept override {
        return "I32";
    }

    [[nodiscard]] Value clone() const override {
        return make(m_value, value_category());
    }

    [[nodiscard]] Value unary_plus() const override {
        return make(m_value, ValueCategory::Rvalue);
    }

    [[nodiscard]] Value unary_minus() const override {
        return make(-m_value, ValueCategory::Rvalue);
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override {
        if (other->is_integer_value()) {
            return make(value() + other->as_integer_value().value(), ValueCategory::Rvalue);
        }
        if (other->is_string_value()) {
            return String::make(string_representation() + other->string_representation(), ValueCategory::Rvalue);
        }
        return BasicValue::binary_plus(other); // throws
    }

    [[nodiscard]] Value binary_minus(Value const& other) const override {
        if (other->is_integer_value()) {
            return make(value() - other->as_integer_value().value(), ValueCategory::Rvalue);
        }
        return BasicValue::binary_plus(other); // throws
    }

    [[nodiscard]] Value equals(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() == other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value not_equals(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() != other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value greater_than(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() > other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value greater_or_equals(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() >= other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value less_than(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() < other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value less_or_equals(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() <= other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value multiply(Value const& other) const override {
        if (other->is_integer_value()) {
            return make(value() * other->as_integer_value().value(), ValueCategory::Rvalue);
        }

        if (other->is_string_value()) {
            auto result = std::string{};
            for (ValueType i = 0; i < value(); ++i) {
                result += other->as_string_value().value();
            }
            return String::make(std::move(result), ValueCategory::Rvalue);
        }

        return BasicValue::multiply(other->clone()); // throws
    }

    [[nodiscard]] Value divide(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::divide(other->clone()); // throws
        }
        return make(value() / other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value mod(Value const& other) const override {
        if (not other->is_integer_value()) {
            return BasicValue::mod(other->clone()); // throws
        }
        return make(value() % other->as_integer_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value range(Value const& other, bool end_is_inclusive) const override;

    void assign(Value const& other) override {
        if (not is_lvalue()) {
            throw LvalueRequired{};
        }
        if (not other->is_integer_value()) {
            return BasicValue::assign(other->clone()); // throws
        }
        m_value = other->as_integer_value().value();
    }
};