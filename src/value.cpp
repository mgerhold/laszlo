#include "value.hpp"

[[nodiscard]] Value Integer::binary_plus(Value const& other) const {
    if (other->is_integer_value()) {
        return make_integer_value(value() + other->as_integer_value().value());
    }
    if (other->is_string_value()) {
        return make_string_value(string_representation() + other->string_representation());
    }
    return BasicValue::binary_plus(other); // throws
}

[[nodiscard]] Value Integer::binary_minus(Value const& other) const {
    if (other->is_integer_value()) {
        return make_integer_value(value() - other->as_integer_value().value());
    }
    return BasicValue::binary_plus(other); // throws
}

[[nodiscard]] Value Integer::equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<Bool>(value() == other->as_integer_value().value());
}

[[nodiscard]] Value Integer::not_equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<Bool>(value() != other->as_integer_value().value());
}

[[nodiscard]] Value Integer::greater_than(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<Bool>(value() > other->as_integer_value().value());
}

[[nodiscard]] Value Integer::greater_or_equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<Bool>(value() >= other->as_integer_value().value());
}

[[nodiscard]] Value Integer::less_than(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<Bool>(value() < other->as_integer_value().value());
}

[[nodiscard]] Value Integer::less_or_equals(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::equals(other); // throws
    }
    return std::make_unique<Bool>(value() <= other->as_integer_value().value());
}

[[nodiscard]] Value Integer::assign(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::assign(other->clone()); // throws
    }
    return other->clone();
}

[[nodiscard]] Value Integer::multiply(Value const& other) const {
    if (other->is_integer_value()) {
        return make_integer_value(value() * other->as_integer_value().value());
    }

    if (other->is_string_value()) {
        auto result = std::string{};
        for (ValueType i = 0; i < value(); ++i) {
            result += other->as_string_value().value();
        }
        return make_string_value(std::move(result));
    }

    return BasicValue::assign(other->clone()); // throws
}

[[nodiscard]] Value Integer::divide(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::assign(other->clone()); // throws
    }
    return make_integer_value(value() / other->as_integer_value().value());
}

[[nodiscard]] Value Integer::mod(Value const& other) const {
    if (not other->is_integer_value()) {
        return BasicValue::assign(other->clone()); // throws
    }
    return make_integer_value(value() % other->as_integer_value().value());
}

[[nodiscard]]Value Integer::range(Value const& other, bool const end_is_inclusive) const {
    if (not other->is_integer_value()) {
        return BasicValue::assign(other->clone()); // throws
    }
    return std::make_unique<RangeIterator>(clone(), end_is_inclusive, other->clone());
}

[[nodiscard]] Value String::multiply(Value const& other) const {
    if (other->is_integer_value()) {
        return other->multiply(clone());
    }
    return BasicValue::multiply(other); // throws
}

[[nodiscard]] Value Bool::logical_and(Value const& other) const {
    if (not other->is_bool_value()) {
        return BasicValue::logical_and(other); // throws
    }
    return make_bool_value(value() and other->as_bool_value().value());
}

[[nodiscard]] Value Bool::logical_or(Value const& other) const {
    if (not other->is_bool_value()) {
        return BasicValue::logical_and(other); // throws
    }
    return make_bool_value(value() or other->as_bool_value().value());
}
