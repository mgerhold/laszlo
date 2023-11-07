#include "integer.hpp"
#include "range.hpp"

namespace values {

    [[nodiscard]] Value Integer::range(Value const& other, bool end_is_inclusive) const {
        if (not other->is_integer_value()) {
            return BasicValue::range(other->clone(), end_is_inclusive); // throws
        }
        return Range::make(as_rvalue(), end_is_inclusive, other->as_rvalue(), ValueCategory::Rvalue);
    }

} // namespace values
