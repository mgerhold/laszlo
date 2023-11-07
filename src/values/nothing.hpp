#pragma once

#include "value.hpp"

namespace values {

    class Nothing final : public BasicValue {
    public:
        using BasicValue::BasicValue;

        [[nodiscard]] static Value make(ValueCategory const value_category) {
            return std::make_shared<Nothing>(value_category);
        }

        [[nodiscard]] std::string string_representation() const override {
            return "nothing";
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_nothing();
        }

        [[nodiscard]] Value clone() const override {
            return make(value_category());
        }
    };

} // namespace values
