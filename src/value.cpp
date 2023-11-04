#include "value.hpp"

[[nodiscard]] Value IntegerValue::binary_plus(Value const& other) const {
    if (auto const other_int = dynamic_cast<IntegerValue const*>(other.get()); other_int != nullptr) {
        return std::make_unique<IntegerValue>(m_value + other_int->m_value);
    } else if (auto const other_string = dynamic_cast<StringValue const*>(other.get()); other_string != nullptr) {
        return std::make_unique<StringValue>(string_representation() + other_string->string_representation());
    }
    return BasicValue::binary_plus(other); // throws
}

[[nodiscard]] Value IntegerValue::binary_minus(Value const& other) const {
    auto const other_ptr = dynamic_cast<IntegerValue const*>(other.get());
    if (other_ptr == nullptr) {
        return BasicValue::binary_plus(other);
    }
    return std::make_unique<IntegerValue>(m_value - other_ptr->m_value);
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
    return other->clone();
}
