#include "string_iterator.hpp"

#include "sentinel.hpp"
#include "string.hpp"

namespace values {
    [[nodiscard]] Value StringIterator::next() {
        if (m_current_index >= m_string->as_string().length()) {
            return Sentinel::make(ValueCategory::Rvalue);
        }
        return m_string->as_string().at(m_current_index++);
    }
}
