#pragma once

#include "expression.hpp"

namespace expressions {
    class BinaryOperator final : public Expression {
    public:
        enum class Kind {
            Plus,
            Minus,
            Equality,
            Inequality,
            LessThan,
            LessOrEqual,
            GreaterThan,
            GreaterOrEqual,
            And,
            Or,
            Mod,
            Multiply,
            Divide,
        };

    private:
        std::unique_ptr<Expression> m_left;
        Kind m_kind;
        std::unique_ptr<Expression> m_right;

    public:
        BinaryOperator(std::unique_ptr<Expression> left, Kind const kind, std::unique_ptr<Expression> right)
            : m_left{ std::move(left) },
              m_kind{ kind },
              m_right{ std::move(right) } { }

        [[nodiscard]] values::Value evaluate(ScopeStack& scope_stack) const override {
            switch (m_kind) {
                case Kind::Plus:
                    return m_left->evaluate(scope_stack)->binary_plus(m_right->evaluate(scope_stack));
                case Kind::Minus:
                    return m_left->evaluate(scope_stack)->binary_minus(m_right->evaluate(scope_stack));
                case Kind::Equality:
                    return m_left->evaluate(scope_stack)->equals(m_right->evaluate(scope_stack));
                case Kind::Inequality:
                    return m_left->evaluate(scope_stack)->not_equals(m_right->evaluate(scope_stack));
                case Kind::LessThan:
                    return m_left->evaluate(scope_stack)->less_than(m_right->evaluate(scope_stack));
                case Kind::LessOrEqual:
                    return m_left->evaluate(scope_stack)->less_or_equals(m_right->evaluate(scope_stack));
                case Kind::GreaterThan:
                    return m_left->evaluate(scope_stack)->greater_than(m_right->evaluate(scope_stack));
                case Kind::GreaterOrEqual:
                    return m_left->evaluate(scope_stack)->greater_or_equals(m_right->evaluate(scope_stack));
                case Kind::And:
                    return m_left->evaluate(scope_stack)->logical_and(m_right->evaluate(scope_stack));
                case Kind::Or:
                    return m_left->evaluate(scope_stack)->logical_or(m_right->evaluate(scope_stack));
                case Kind::Mod:
                    return m_left->evaluate(scope_stack)->mod(m_right->evaluate(scope_stack));
                case Kind::Multiply:
                    return m_left->evaluate(scope_stack)->multiply(m_right->evaluate(scope_stack));
                case Kind::Divide:
                    return m_left->evaluate(scope_stack)->divide(m_right->evaluate(scope_stack));
                default:
                    assert(false and "unreachable");
                    return {};
            }
        }

        [[nodiscard]] SourceLocation source_location() const override {
            return SourceLocation::from_range(m_left->source_location(), m_right->source_location());
        }
    };
}