#include "char.hpp"
#include "bool.hpp"
#include "integer.hpp"

namespace values {
    [[nodiscard]] Value Char::equals(Value const& other) const {
        if (not other->is_char_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() == other->as_char_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Char::not_equals(Value const& other) const {
        return Bool::make(not equals(other)->as_bool_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Char::binary_plus(Value const& other) const {
        if (other->is_integer_value()) {
            return make(value() + other->as_integer_value().value(), ValueCategory::Rvalue);
        }
        if (other->is_string_value()) {
            return String::make(
                    static_cast<char>(value()) + other->as_string().string_representation(),
                    ValueCategory::Rvalue
            );
        }
        return BasicValue::binary_plus(other); // throws
    }

    [[nodiscard]] Value Char::binary_minus(Value const& other) const {
        if (not other->is_char_value()) {
            return BasicValue::binary_minus(other); // throws
        }
        return Integer::make(
                static_cast<Integer::ValueType>(value())
                        - static_cast<Integer::ValueType>(other->as_char_value().value()),
                ValueCategory::Rvalue
        );
    }

    [[nodiscard]] Value Char::greater_than(Value const& other) const {
        if (not other->is_char_value()) {
            return BasicValue::greater_than(other); // throws
        }
        return Bool::make(value() > other->as_char_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Char::greater_or_equals(Value const& other) const {
        if (not other->is_char_value()) {
            return BasicValue::greater_or_equals(other); // throws
        }
        return Bool::make(value() >= other->as_char_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Char::less_than(Value const& other) const {
        if (not other->is_char_value()) {
            return BasicValue::less_than(other); // throws
        }
        return Bool::make(value() < other->as_char_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Char::less_or_equals(Value const& other) const {
        if (not other->is_char_value()) {
            return BasicValue::less_or_equals(other); // throws
        }
        return Bool::make(value() <= other->as_char_value().value(), ValueCategory::Rvalue);
    }

    void Char::assign(Value const& other) {
        if (not is_lvalue()) {
            throw LvalueRequired{};
        }
        if (not other->is_char_value()) {
            BasicValue::assign(other); // throw
        }
        m_value = other->as_char_value().value();
    }
} // namespace values
