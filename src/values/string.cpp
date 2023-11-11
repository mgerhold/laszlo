#include "string.hpp"
#include "bool.hpp"
#include "char.hpp"
#include "integer.hpp"

namespace values {

    [[nodiscard]] String::ValueType String::to_value_type(std::string_view const value) {
        auto chars = std::vector<Value>{};
        chars.reserve(value.length());
        for (auto const c : value) {
            chars.push_back(Char::make(c, ValueCategory::Lvalue));
        }
        return chars;
    }

    [[nodiscard]] Value String::equals(Value const& other) const {
        if (not other->is_string_value()) {
            return BasicValue::equals(other); // throws
        }
        return Bool::make(value() == other->as_string().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value String::not_equals(Value const& other) const {
        return Bool::make(not equals(other)->as_bool_value().value(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value String::subscript(Value const& index) const {
        if (not index->is_integer_value()) {
            return BasicValue::subscript(index); // throws
        }
        auto const index_value = index->as_integer_value().value();
        if (index_value < 0 or index_value >= m_value.size()) {
            throw IndexOutOfBounds{ index_value, static_cast<Integer::ValueType>(m_value.size()) };
        }
        assert(m_value.at(index_value)->is_lvalue());
        return m_value.at(index_value);
    }

} // namespace values
