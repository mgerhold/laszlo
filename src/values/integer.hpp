#pragma once

#include "bool.hpp"
#include "string.hpp"
#include "value.hpp"
#include <memory>
#include <string>

namespace values {

    class Integer final : public BasicValue {
    public:
        using ValueType = std::int32_t;

    private:
        ValueType m_value;

    public:
        explicit Integer(ValueType const value, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_value{ value } { }

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

        [[nodiscard]] types::Type type() const noexcept override {
            return std::make_unique<types::I32>();
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
                    result += other->as_string().string_representation();
                }
                return String::make(std::move(result), ValueCategory::Rvalue);
            }

            return BasicValue::multiply(other->clone()); // throws
        }

        [[nodiscard]] Value divide(Value const& other) const override {
            if (not other->is_integer_value()) {
                return BasicValue::divide(other->clone()); // throws
            }
            auto const rhs = other->as_integer_value().value();
            if (rhs == 0) {
                throw DivisionByZero{};
            }
            return make(value() / rhs, ValueCategory::Rvalue);
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

} // namespace values
