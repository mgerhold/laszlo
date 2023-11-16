#pragma once

#include "iterator.hpp"
#include "string.hpp"

namespace values {
    class StringIterator final : public Iterator {
    private:
        Value m_string;
        std::size_t m_current_index;

    public:
        StringIterator(Value string, std::size_t const current_index, ValueCategory const value_category)
            : Iterator{ value_category },
              m_string{ std::move(string) },
              m_current_index{ current_index } {
            assert(m_string->is_string_value());
        }

        [[nodiscard]] static Value make(Value string, std::size_t current_index, ValueCategory const value_category) {
            return std::make_shared<StringIterator>(std::move(string), current_index, value_category);
        }

        [[nodiscard]] std::string string_representation() const override {
            return std::format(
                    "StringIterator({}, {}/{})",
                    m_string->string_representation(),
                    m_current_index,
                    m_string->as_string().length()
            );
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_string_iterator();
        }

        [[nodiscard]] Value clone() const override {
            return make(m_string, m_current_index, value_category());
        }

        [[nodiscard]] Value next() override;
    };
} // namespace values
