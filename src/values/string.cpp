#include "string.hpp"
#include "bool.hpp"

namespace values {

    [[nodiscard]] Value String::equals(Value const& other) const {
        if (not other->is_string_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() == other->as_string().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value String::not_equals(Value const& other) const {
        return Bool::make(not equals(other)->as_bool_value().value(), ValueCategory::Rvalue);
    }

} // namespace values
