#pragma once

#include "expression.hpp"

namespace expressions {
    class Range final : public Expression {
    private:
        std::unique_ptr<Expression> m_start;
        bool m_end_is_inclusive;
        std::unique_ptr<Expression> m_end;

    public:
        Range(std::unique_ptr<Expression> start, bool const end_is_inclusive, std::unique_ptr<Expression> end)
            : m_start{ std::move(start) },
              m_end_is_inclusive{ end_is_inclusive },
              m_end{ std::move(end) } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            return m_start->evaluate(scope_stack)->range(m_end->evaluate(scope_stack), m_end_is_inclusive);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_start->source_location(), m_end->source_location());
        }
    };
}