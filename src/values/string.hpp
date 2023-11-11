#pragma once

#include "char.hpp"
#include "value.hpp"

namespace values {

    class String final : public BasicValue {
    public:
        using ValueType = std::vector<Value>;

    private:
        ValueType m_value;

        [[nodiscard]] static ValueType to_value_type(std::string_view value);

    public:
        explicit String(std::string_view const value, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_value{ to_value_type(value) } { }

        [[nodiscard]] static Value make(std::string_view const value, ValueCategory const value_category) {
            return std::make_shared<String>(value, value_category);
        }

        [[nodiscard]] bool is_string_value() const override {
            return true;
        }

        [[nodiscard]] String const& as_string() const override {
            return *this;
        }

        [[nodiscard]] ValueType const& value() const {
            return m_value;
        }

        [[nodiscard]] std::string string_representation() const override {
            auto result = std::string{};
            for (auto const& c : m_value) {
                assert(c->is_char_value());
                result += c->as_char_value().value();
            }
            return result;
        }

        [[nodiscard]] types::Type type() const noexcept override {
            return std::make_unique<types::String>();
        }

        [[nodiscard]] Value binary_plus(Value const& other) const override {
            return make(string_representation() + other->string_representation(), ValueCategory::Rvalue);
        }

        [[nodiscard]] Value clone() const override {
            return make(string_representation(), value_category());
        }

        [[nodiscard]] Value multiply(Value const& other) const override {
            if (other->is_integer_value()) {
                return other->as_rvalue()->multiply(clone());
            }
            return BasicValue::multiply(other); // throws
        }

        void assign(Value const& other) override {
            if (not other->is_string_value()) {
                BasicValue::assign(other); // throws
            }
            m_value = other->as_string().m_value;
        }

        [[nodiscard]] Value equals(Value const& other) const override;

        [[nodiscard]] Value not_equals(Value const& other) const override;

        [[nodiscard]] Value subscript(Value const& index) const override;
    };

} // namespace values
