#pragma once

#include "range_interator.hpp"
#include "value.hpp"

namespace values {

    class Range final : public BasicValue {
    private:
        Value m_start;
        bool m_end_is_inclusive;
        Value m_end;

    public:
        Range(Value start, bool const end_is_inclusive, Value end, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_start{ start->is_rvalue() ? std::move(start) : start->as_rvalue() },
              m_end_is_inclusive{ end_is_inclusive },
              m_end{ end->is_rvalue() ? std::move(end) : end->as_rvalue() } {
            assert(m_start->is_integer_value());
            assert(m_end->is_integer_value());
        }

        // clang-format off
    [[nodiscard]] static Value make(
            Value start,
            bool const end_is_inclusive,
            Value end,
            ValueCategory const value_category
    ) { // clang-format on
            return std::make_shared<Range>(std::move(start), end_is_inclusive, std::move(end), value_category);
        }

        [[nodiscard]] std::string string_representation() const override {
            return std::format(
                    "{}..{}{}",
                    m_start->string_representation(),
                    m_end_is_inclusive ? "=" : "",
                    m_end->string_representation()
            );
        }

        [[nodiscard]] std::string type_name() const override {
            return std::format("Range({}..{})", m_start->type_name(), m_end->type_name());
        }

        [[nodiscard]] Value clone() const override {
            return make(m_start, m_end_is_inclusive, m_end, value_category());
        }

        [[nodiscard]] Value iterator() override {
            return RangeIterator::make(
                    m_start->as_rvalue(),
                    m_end_is_inclusive,
                    m_end->as_rvalue(),
                    m_start->as_integer_value().value(),
                    ValueCategory::Rvalue
            );
        }
    };

} // namespace values
