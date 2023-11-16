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
        return Integer::make(static_cast<Integer::ValueType>(m_elements.size()), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value Array::equals(Value const& other) const {
        if (type() != other->type()) {
            return BasicValue::equals(other); // throws
        }
        if (m_elements.size() != other->as_array().m_elements.size()) {
            return Bool::make(false, ValueCategory::Rvalue);
        }
        for (auto i = std::size_t{ 0 }; i < m_elements.size(); ++i) {
            if (not m_elements.at(i)->equals(other->as_array().m_elements.at(i))) {
                return Bool::make(false, ValueCategory::Rvalue);
            }
        }
        return Bool::make(true, ValueCategory::Rvalue);
    }

} // namespace values
