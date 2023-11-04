#include "value.hpp"

[[nodiscard]] Value IntegerValue::binary_plus(Value const& other) const {
    if (other->is_integer_value()) {
        return make_integer_value(m_value + other->as_integer_value().m_value);
    }
    if (other->is_string_value()) {
        return make_string_value(string_representation() + other->string_representation());
    }
    return BasicValue::binary_plus(other); // throws
}

[[nodiscard]] Value IntegerValue::binary_minus(Value const& other) const {
    if (other->is_integer_value()) {
        return make_integer_value(m_value - other->as_integer_value().m_value);
    }
    return BasicValue::binary_plus(other); // throws
}

[[nodiscard]] Value IntegerValue::equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<BoolValue>(m_value == other->as_integer_value().m_value);
}

[[nodiscard]] Value IntegerValue::not_equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<BoolValue>(m_value != other->as_integer_value().m_value);
}

[[nodiscard]] Value IntegerValue::greater_than(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<BoolValue>(m_value > other->as_integer_value().m_value);
}

[[nodiscard]] Value IntegerValue::greater_or_equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<BoolValue>(m_value >= other->as_integer_value().m_value);
}

[[nodiscard]] Value IntegerValue::less_than(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<BoolValue>(m_value < other->as_integer_value().m_value);
}

[[nodiscard]] Value IntegerValue::less_or_equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<BoolValue>(m_value <= other->as_integer_value().m_value);
}

[[nodiscard]] Value IntegerValue::assign(Value other) const {
    if (not other->is_integer_value()) {
        return BasicValue::assign(other->clone()); // throws
    }
    return other;
}
