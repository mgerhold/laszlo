#include "array.hpp"
#include "array_iterator.hpp"

namespace values {

    [[nodiscard]] Value Array::iterator() {
        return ArrayIterator::make(shared_from_this(), ValueCategory::Rvalue);
    }

}