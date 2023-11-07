#pragma once

#include "value.hpp"

namespace values {

    class Sentinel final : public BasicValue {
    public:
        explicit Sentinel(ValueCategory const value_category) : BasicValue{ value_category } { }

        [[nodiscard]] static Value make(ValueCategory const value_category) {
            return std::make_shared<Sentinel>(value_category);
        }

        [[nodiscard]] bool is_sentinel() const override {
            return true;
        }

        [[nodiscard]] std::string string_representation() const override {
            return "Sentinel";
        }

        [[nodiscard]] std::string type_name() const override {
            return "Sentinel";
        }

        [[nodiscard]] Value clone() const override {
            return make(value_category());
        }
    };

} // namespace values
