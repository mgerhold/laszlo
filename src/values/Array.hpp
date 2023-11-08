#pragma once

#include "integer.hpp"
#include "value.hpp"

namespace values {

    class Array final : public BasicValue, public std::enable_shared_from_this<Array> {
    public:
        using ValueType = std::vector<Value>;

    private:
        ValueType m_elements;

    public:
        explicit Array(std::vector<Value> elements, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_elements{ std::move(elements) } { }

        [[nodiscard]] static Value make(std::vector<Value> elements, ValueCategory const value_category) {
            return std::make_shared<Array>(std::move(elements), value_category);
        }

        [[nodiscard]] bool is_array() const override {
            return true;
        }

        [[nodiscard]] Array const& as_array() const override {
            return *this;
        }

        [[nodiscard]] ValueType const& value() const {
            return m_elements;
        }

        [[nodiscard]] std::string string_representation() const override {
            auto result = std::string{ "[" };
            for (std::size_t i = 0; i < m_elements.size(); ++i) {
                result += m_elements.at(i)->string_representation();
                if (i < m_elements.size() - 1) {
                    result += ", ";
                }
            }
            return result + "]";
        }

        [[nodiscard]] types::Type type() const override {
            if (m_elements.empty()) {
                return std::make_unique<types::Array>(std::make_unique<types::Unspecified>());
            }
            return std::make_unique<types::Array>(m_elements.front()->type());
        }

        [[nodiscard]] Value clone() const override {
            auto values = std::vector<Value>{};
            values.reserve(m_elements.size());
            for (auto const& value : m_elements) {
                values.push_back(value->clone());
            }
            return make(std::move(values), value_category());
        }

        [[nodiscard]] Value binary_plus(Value const& other) const override {
            if (value().empty()) {
                return other->as_rvalue();
            }

            if (other->as_array().value().empty()) {
                return as_rvalue();
            }

            if (type() != other->type()) {
                return BasicValue::binary_plus(other); // throws
            }

            auto values = ValueType{};
            values.reserve(value().size() + other->as_array().value().size());
            for (auto const& current : value()) {
                // arrays always contain lvalues
                assert(current->is_lvalue());
                // the new array gets its own copy of the value (but still an lvalue)
                values.push_back(current->clone());
            }
            for (auto const& current : other->as_array().value()) {
                assert(current->is_lvalue());
                values.push_back(current->clone());
            }

            /* The resulting array itself is an rvalue (containing lvalues), so assigning to a
             * temporary array (e.g. array literal) is possible, but has no effect.
             * But this has the benefit of not having to recursively promote all values of an
             * array to lvalues when the value itself becomes an lvalue (e.g. when being assigned
             * to a variable. */
            return make(std::move(values), ValueCategory::Rvalue);
        }

        [[nodiscard]] Value subscript(Value const& index) const override {
            if (not index->is_integer_value()) {
                throw UnableToSubscript{ index->type(), type() };
            }
            if (index->as_integer_value().value() < 0 or index->as_integer_value().value() >= m_elements.size()) {
                throw IndexOutOfBounds{ index->as_integer_value().value(),
                                        static_cast<Integer::ValueType>(m_elements.size()) };
            }
            return m_elements.at(index->as_integer_value().value());
        }

        void assign(Value const& other) override {
            if (not other->is_array()) {
                BasicValue::assign(other); // throws
            }
            if (other->as_array().value().empty()) {
                return;
            }
            /* todo: the type of values contained in an array should not be allowed
         *       to change after it first had some values */
            m_elements.clear();
            for (auto const& current : other->as_array().value()) {
                m_elements.push_back(current->clone());
                assert(m_elements.back()->is_lvalue());
            }
        }

        [[nodiscard]] Value iterator() override;
        Value member_access(Token member) const override;
    };

} // namespace values
