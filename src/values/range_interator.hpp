#pragma once

#include "integer.hpp"
#include "iterator.hpp"
#include "sentinel.hpp"

class RangeIterator : public Iterator {
private:
    enum class Direction {
        Increasing,
        Decreasing,
    };

    Value m_start;
    bool m_end_is_inclusive;
    Value m_end;
    Value m_current;
    Direction m_direction;

public:
    RangeIterator(
            Value const& start,
            bool const end_is_inclusive,
            Value const& end,
            Value const& current,
            ValueCategory const value_category
    )
        : Iterator{ value_category },
          m_start{ start->clone() },
          m_end_is_inclusive{ end_is_inclusive },
          m_end{ end->clone() },
          m_current{ current->clone() } {
        assert(m_start->is_integer_value());
        assert(m_end->is_integer_value());

        auto const start_value = m_start->as_integer_value().value();
        auto const end_value = m_end->as_integer_value().value();

        if (start_value > end_value) {
            m_direction = Direction::Decreasing;
        } else {
            m_direction = Direction::Increasing;
        }
    }

    // clang-format off
    [[nodiscard]] static Value make(
            Value start,
            bool const end_is_inclusive,
            Value end,
            Value current,
            ValueCategory const value_category
    ) { // clang-format on
        return std::make_shared<RangeIterator>(
                std::move(start),
                end_is_inclusive,
                std::move(end),
                std::move(current),
                value_category
        );
    }

    [[nodiscard]] std::string string_representation() const override {
        return std::format("{}..{}", m_start->string_representation(), m_end->string_representation());
    }

    [[nodiscard]] std::string type_name() const override {
        return "Range";
    }

    [[nodiscard]] Value clone() const override {
        return make(m_start->clone(), m_end_is_inclusive, m_end->clone(), m_current->clone(), value_category());
    }

    [[nodiscard]] Iterator const& as_iterator() const override {
        return *this;
    }

    [[nodiscard]] std::pair<Value, Value> next() const override {
        auto const current_value = m_current->as_integer_value().value();
        auto const end_value = m_end->as_integer_value().value();
        // clang-format off
        auto const exclusive_bound = (
                m_end_is_inclusive ? (
                    m_direction == Direction::Increasing ? end_value + 1 : end_value - 1
                ) : end_value
            );
        // clang-format on
        if ((m_direction == Direction::Increasing and current_value >= exclusive_bound)
            or (m_direction == Direction::Decreasing and current_value <= exclusive_bound)) {
            return { as_rvalue(), Sentinel::make(ValueCategory::Rvalue) };
        }
        auto const new_value = m_current->as_integer_value().value() + (m_direction == Direction::Increasing ? 1 : -1);
        return { make(m_start->clone(),
                      m_end_is_inclusive,
                      m_end->clone(),
                      Integer::make(new_value, ValueCategory::Lvalue),
                      ValueCategory::Rvalue),
                 Integer::make(current_value, ValueCategory::Rvalue) };
    }
};
