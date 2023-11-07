#include "array.hpp"
#include "array_iterator.hpp"

namespace values {

    [[nodiscard]] Value Array::iterator() const {
        return ArrayIterator::make(as_rvalue(), ValueCategory::Rvalue);
    }

}