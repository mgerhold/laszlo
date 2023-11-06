#pragma once

#include "value.hpp"

class Iterator : public BasicValue {
protected:
    explicit Iterator(ValueCategory const value_category) : BasicValue{ value_category } { }

public:
    [[nodiscard]] bool is_iterator() const final {
        return true;
    }

    [[nodiscard]] Iterator const& as_iterator() const override = 0;

    [[nodiscard]] virtual std::pair<Value, Value> next() const = 0;
};
