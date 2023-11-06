#include "integer.hpp"
#include "range_interator.hpp"

[[nodiscard]] Value Integer::range(Value const& other, bool end_is_inclusive) const {
    if (not other->is_integer_value()) {
        return BasicValue::range(other->clone(), end_is_inclusive); // throws
    }
    auto start = clone();
    if (start->is_rvalue()) {
        start->promote_to_lvalue();
    }

    auto current = start->clone();

    auto end = other->clone();
    if (end->is_rvalue()) {
        end->promote_to_lvalue();
    }
    return RangeIterator::make(
            std::move(start),
            end_is_inclusive,
            std::move(end),
            std::move(current),
            ValueCategory::Rvalue
    );
}
