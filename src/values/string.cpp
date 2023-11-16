#include "string.hpp"
#include "bool.hpp"
#include "char.hpp"
#include "integer.hpp"
#include "string_iterator.hpp"
#include <sstream>

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
        return Bool::make(string_representation() == other->string_representation(), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value String::subscript(Value const& index) const {
        if (not index->is_integer_value()) {
            return BasicValue::subscript(index); // throws
        }
        auto const index_value = index->as_integer_value().value();
        if (index_value < 0 or index_value >= m_chars.size()) {
            throw IndexOutOfBounds{ index_value, static_cast<Integer::ValueType>(m_chars.size()) };
        }
        assert(m_chars.at(index_value)->is_lvalue());
        return m_chars.at(index_value);
    }

    [[nodiscard]] Value String::member_access(Token const member) const {
        if (member.lexeme() != "size" and member.lexeme() != "length") {
            return BasicValue::member_access(member); // throw
        }
        return Integer::make(static_cast<Integer::ValueType>(m_chars.size()), ValueCategory::Rvalue);
    }

    [[nodiscard]] Value String::iterator() {
        return StringIterator::make(shared_from_this(), 0, ValueCategory::Rvalue);
    }

    [[nodiscard]] Value String::cast(types::Type const& target_type) const {
        if (target_type == types::make_i32()) {
            auto stream = std::istringstream{ string_representation() };
            auto value = Integer::ValueType{};
            stream >> value;
            if (not stream or not stream.eof()) {
                throw CastError{ string_representation(), target_type->to_string() };
            }
            return Integer::make(value, ValueCategory::Rvalue);
        }
        return BasicValue::cast(target_type);
    }

} // namespace values
