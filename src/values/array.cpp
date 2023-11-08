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

} // namespace values
