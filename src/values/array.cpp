#include "array.hpp"
#include "array_iterator.hpp"

namespace values {

    [[nodiscard]] Value Array::iterator() {
        return ArrayIterator::make(shared_from_this(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Array::member_access(Token member) const {
        if (member.type != TokenType::Identifier or member.lexeme() != "size") {
            return BasicValue::member_access(member); // throws
        }
        return values::Integer::make(static_cast<values::Integer::ValueType>(m_elements.size()), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Array::equals(Value const& other) const {
        if (type() != other->type()) {
            return BasicValue::equals(other); // throws
        }
        if (m_elements.size() != other->as_array().m_elements.size()) {
            return values::Bool::make(false, ValueCategory::Rvalue);
        }
        for (auto i = std::size_t{ 0 }; i < m_elements.size(); ++i) {
            if (not m_elements.at(i)->equals(other->as_array().m_elements.at(i))) {
                return values::Bool::make(false, ValueCategory::Rvalue);
            }
        }
        return values::Bool::make(true, ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Array::not_equals(Value const& other) const {
        return values::Bool::make(not equals(other)->as_bool_value().value(), ValueCategory::Rvalue);
    }

} // namespace values
