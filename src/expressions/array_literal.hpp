#pragma once

#include "expression.hpp"
#include "../values/array.hpp"

namespace expressions {
    class ArrayLiteral final : public Expression {
    private:
        Token m_opening_bracket;
        std::vector<std::unique_ptr<Expression>> m_values;
        Token m_closing_bracket;

    public:
        ArrayLiteral(
                Token const opening_bracket,
                std::vector<std::unique_ptr<Expression>> values,
                Token const closing_bracket
        )
            : m_opening_bracket{ opening_bracket },
              m_values{ std::move(values) },
              m_closing_bracket{ closing_bracket } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            auto elements = std::vector<values::Value>{};
            elements.reserve(m_values.size());
            for (auto const& value : m_values) {
                auto evaluated = value->evaluate(scope_stack);
                if (evaluated->is_lvalue()) {
                    evaluated = evaluated->clone();
                } else {
                    evaluated->promote_to_lvalue();
                }
                elements.push_back(std::move(evaluated));
            }

            if (not elements.empty()) {
                for (std::size_t i = 1; i < elements.size(); ++i) {
                    if (elements.at(i)->type_name() != elements.front()->type_name()) {
                        throw TypeMismatch{ m_values.at(i)->source_location(),
                                            elements.front()->type_name(),
                                            elements.at(i)->type_name() };
                    }
                }
            }

            for ([[maybe_unused]] auto const& element : elements) {
                assert(element->is_lvalue());
            }

            return values::Array::make(std::move(elements), values::ValueCategory::Rvalue);
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_opening_bracket.source_location, m_closing_bracket.source_location);
        }
    };
}