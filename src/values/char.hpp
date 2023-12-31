#pragma once

#include "value.hpp"
#include <cstdint>

namespace values {
    class Char final : public BasicValue {
    public:
        using ValueType = std::uint8_t;

    private:
        ValueType m_value;

    public:
        Char(ValueType const value, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_value{ value } { }

        [[nodiscard]] static Value make(ValueType const value, ValueCategory const value_category) {
            return std::make_shared<Char>(value, value_category);
        }

        [[nodiscard]] ValueType value() const {
            return m_value;
        }

        [[nodiscard]] bool is_char_value() const override {
            return true;
        }

        [[nodiscard]] Char const& as_char_value() const override {
            return *this;
        }

        [[nodiscard]] std::string string_representation() const override {
            return std::format("{}", static_cast<char>(m_value));
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_char();
        }

        [[nodiscard]] Value clone() const override {
            return make(m_value, value_category());
        }

        [[nodiscard]] Value equals(Value const& other) const override;

        [[nodiscard]] Value binary_plus(Value const& other) const override;

        [[nodiscard]] Value binary_minus(Value const& other) const override;

        [[nodiscard]] Value greater_than(Value const& other) const override;

        [[nodiscard]] Value greater_or_equals(Value const& other) const override;

        [[nodiscard]] Value less_than(Value const& other) const override;

        [[nodiscard]] Value less_or_equals(Value const& other) const override;

        void assign(Value const& other) override;
    };
} // namespace values
