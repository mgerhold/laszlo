#pragma once

#include "array.hpp"
#include "integer.hpp"
#include "iterator.hpp"
#include "sentinel.hpp"

namespace values {

    class ArrayIterator final : public Iterator {
    private:
        Value m_array;
        Integer::ValueType m_current_index{ 0 };

    public:
        ArrayIterator(Value array, ValueCategory const value_category)
            : Iterator{ value_category },
              m_array{ std::move(array) } { }

        [[nodiscard]] static Value make(Value array, ValueCategory const value_category) {
            return std::make_shared<ArrayIterator>(std::move(array), value_category);
        }

        [[nodiscard]] Value next() override {
            if (m_current_index >= m_array->as_array().value().size()) {
                return Sentinel::make(ValueCategory::Rvalue);
            }
            auto const old_index = m_current_index;
            ++m_current_index;
            return m_array->as_array().value().at(old_index);
        }

        [[nodiscard]] std::string string_representation() const override {
            return std::format(
                    "ArrayIterator({}, {}/{})",
                    m_array->string_representation(),
                    m_current_index,
                    m_array->as_array().value().size()
            );
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_array_iterator(m_array->type());
        }

        [[nodiscard]] Value clone() const override {
            return make(m_array, value_category());
        }
    };

} // namespace values
