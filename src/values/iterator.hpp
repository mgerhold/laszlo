#pragma once

#include "value.hpp"

namespace values {

    class Iterator : public BasicValue {
    protected:
        explicit Iterator(ValueCategory const value_category) : BasicValue{ value_category } { }

    public:
        [[nodiscard]] bool is_iterator() const final {
            return true;
        }

        [[nodiscard]] Iterator& as_iterator() override {
            return *this;
        }

        [[nodiscard]] virtual Value next() = 0;
    };

} // namespace values
