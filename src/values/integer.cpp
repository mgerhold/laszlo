#include "integer.hpp"
#include "range.hpp"

namespace values {

    [[nodiscard]] Value Integer::range(Value const& other, bool end_is_inclusive) const {
        if (not other->is_integer_value()) {
            return BasicValue::range(other->clone(), end_is_inclusive); // throws
        }
        return Range::make(as_rvalue(), end_is_inclusive, other->as_rvalue(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Integer::cast(types::Type const& target_type) const {
        if (target_type == types::make_char()) {
            return Char::make(static_cast<Char::ValueType>(value()), ValueCategory::Rvalue);
        }
        if (target_type == types::make_bool()) {
            return Bool::make(value() != 0, ValueCategory::Rvalue);
        }
        return BasicValue::cast(target_type); // may throw
    }

} // namespace values
