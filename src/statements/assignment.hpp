#pragma once

#include "statement.hpp"

namespace statements {
    class Assignment final : public Statement {
    public:
        enum class Type {
            Equals,
            Plus,
            Minus,
            Asterisk,
            Slash,
        };

    private:
        std::unique_ptr<expressions::Expression> m_lvalue;
        Type m_type;
        std::unique_ptr<expressions::Expression> m_rvalue;

    public:
        Assignment(
                std::unique_ptr<expressions::Expression> lvalue,
                Type const type,
                std::unique_ptr<expressions::Expression> rvalue
        )
            : m_lvalue{ std::move(lvalue) },
              m_type{ type },
              m_rvalue{ std::move(rvalue) } { }

        void execute(ScopeStack& scope_stack) const override {
            switch (m_type) {
                case Type::Equals:
                    m_lvalue->evaluate(scope_stack)->assign(m_rvalue->evaluate(scope_stack));
                    break;
                case Type::Plus: {
                    auto const new_value =
                            m_lvalue->evaluate(scope_stack)->binary_plus(m_rvalue->evaluate(scope_stack));
                    m_lvalue->evaluate(scope_stack)->assign(new_value);
                    break;
                }
                case Type::Minus: {
                    auto const new_value =
                            m_lvalue->evaluate(scope_stack)->binary_minus(m_rvalue->evaluate(scope_stack));
                    m_lvalue->evaluate(scope_stack)->assign(new_value);
                    break;
                }
                case Type::Asterisk: {
                    auto const new_value = m_lvalue->evaluate(scope_stack)->multiply(m_rvalue->evaluate(scope_stack));
                    m_lvalue->evaluate(scope_stack)->assign(new_value);
                    break;
                }
                case Type::Slash: {
                    auto const new_value = m_lvalue->evaluate(scope_stack)->divide(m_rvalue->evaluate(scope_stack));
                    m_lvalue->evaluate(scope_stack)->assign(new_value);
                    break;
                }
                default:
                    throw std::runtime_error{ "unreachable" };
            }
        }
    };
} // namespace statements
