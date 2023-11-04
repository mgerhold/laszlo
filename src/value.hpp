#pragma once

#include "runtime_error.hpp"
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

class BasicValue;
class Integer;
class String;
class Bool;
class Iterator;

using Value = std::unique_ptr<BasicValue>;

class InvalidValueCast : public std::exception {
private:
    std::string m_message;

public:
    explicit InvalidValueCast(std::string_view const target_type)
        : m_message{ std::format("unable to cast value to target type '{}'", target_type) } { }

    [[nodiscard]] char const* what() const noexcept override {
        return m_message.c_str();
    }
};

class BasicValue {
protected:
    BasicValue() = default;

public:
    virtual ~BasicValue() = default;

    [[nodiscard]] virtual bool is_integer_value() const {
        return false;
    }

    [[nodiscard]] virtual Integer const& as_integer_value() const {
        throw InvalidValueCast{ "Integer" };
    }

    [[nodiscard]] virtual bool is_string_value() const {
        return false;
    }

    [[nodiscard]] virtual String const& as_string_value() const {
        throw InvalidValueCast{ "String" };
    }

    [[nodiscard]] virtual bool is_bool_value() const {
        return false;
    }

    [[nodiscard]] virtual Bool const& as_bool_value() const {
        throw InvalidValueCast{ "Bool" };
    }

    [[nodiscard]] virtual bool is_iterator() const {
        return false;
    }

    [[nodiscard]] virtual Iterator const& as_iterator() const {
        throw InvalidValueCast{ "Iterator" };
    }

    [[nodiscard]] virtual bool is_sentinel() const {
        return false;
    }

    [[nodiscard]] virtual std::string string_representation() const = 0;

    [[nodiscard]] virtual Value unary_plus() const {
        throw OperationNotSupportedByType{ "unary_plus", type_name() };
    }

    [[nodiscard]] virtual Value unary_minus() const {
        throw OperationNotSupportedByType{ "unary_minus", type_name() };
    }

    [[nodiscard]] virtual Value binary_plus(Value const& other) const {
        throw OperationNotSupportedByType{ "binary_plus", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value binary_minus(Value const& other) const {
        throw OperationNotSupportedByType{ "binary_minus", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value multiply(Value const& other) const {
        throw OperationNotSupportedByType{ "multiply", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value divide(Value const& other) const {
        throw OperationNotSupportedByType{ "multiply", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value equals(Value const& other) const {
        throw OperationNotSupportedByType{ "equals", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value not_equals(Value const& other) const {
        throw OperationNotSupportedByType{ "not_equals", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value greater_than(Value const& other) const {
        throw OperationNotSupportedByType{ "greater_than", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value greater_or_equals(Value const& other) const {
        throw OperationNotSupportedByType{ "greater_or_equals", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value logical_and(Value const& other) const {
        throw OperationNotSupportedByType{ "logical_and", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value logical_or(Value const& other) const {
        throw OperationNotSupportedByType{ "logical_or", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value mod(Value const& other) const {
        throw OperationNotSupportedByType{ "mod", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value less_than(Value const& other) const {
        throw OperationNotSupportedByType{ "less_than", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value less_or_equals(Value const& other) const {
        throw OperationNotSupportedByType{ "less_or_equals", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value assign(Value const& other) const {
        throw OperationNotSupportedByType{ "assignment", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual Value range(Value const& other, bool end_is_inclusive) const {
        throw OperationNotSupportedByType{ "range", type_name(), other->type_name() };
    }

    [[nodiscard]] virtual char const* type_name() const = 0;

    [[nodiscard]] virtual Value clone() const = 0;
};

class Integer : public BasicValue {
public:
    using ValueType = std::int32_t;

private:
    ValueType m_value;

public:
    explicit Integer(ValueType value) : m_value{ value } { }

    [[nodiscard]] bool is_integer_value() const override {
        return true;
    }

    [[nodiscard]] Integer const& as_integer_value() const override {
        return *this;
    }

    [[nodiscard]] ValueType value() const {
        return m_value;
    }

    [[nodiscard]] std::string string_representation() const override {
        return std::to_string(value());
    }

    [[nodiscard]] char const* type_name() const noexcept override {
        return "I32";
    }

    [[nodiscard]] Value unary_plus() const override {
        return std::make_unique<Integer>(m_value);
    }

    [[nodiscard]] Value unary_minus() const override {
        return std::make_unique<Integer>(-m_value);
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override;

    [[nodiscard]] Value binary_minus(Value const& other) const override;

    [[nodiscard]] Value clone() const override {
        return std::make_unique<Integer>(m_value);
    }

    [[nodiscard]] Value equals(Value const& other) const override;

    [[nodiscard]] Value not_equals(Value const& other) const override;

    [[nodiscard]] Value greater_than(Value const& other) const override;

    [[nodiscard]] Value greater_or_equals(Value const& other) const override;

    [[nodiscard]] Value less_than(Value const& other) const override;

    [[nodiscard]] Value less_or_equals(Value const& other) const override;

    [[nodiscard]] Value assign(Value const& other) const override;

    [[nodiscard]] Value multiply(Value const& other) const override;

    [[nodiscard]] Value divide(Value const& other) const override;

    [[nodiscard]] Value mod(Value const& other) const override;

    [[nodiscard]] Value range(Value const& other, bool end_is_inclusive) const override;
};

class String : public BasicValue {
public:
    using ValueType = std::string;

private:
    ValueType m_value;

public:
    explicit String(ValueType value) : m_value{ std::move(value) } { }

    [[nodiscard]] bool is_string_value() const override {
        return true;
    }

    [[nodiscard]] String const& as_string_value() const override {
        return *this;
    }

    [[nodiscard]] ValueType const& value() const {
        return m_value;
    }

    [[nodiscard]] std::string string_representation() const override {
        return value();
    }

    [[nodiscard]] char const* type_name() const noexcept override {
        return "String";
    }

    [[nodiscard]] Value binary_plus(Value const& other) const override {
        return std::make_unique<String>(m_value + other->string_representation());
    }

    [[nodiscard]] Value clone() const override {
        return std::make_unique<String>(m_value);
    }

    [[nodiscard]] Value multiply(Value const& other) const override;
};

class Bool : public BasicValue {
public:
    using ValueType = bool;

private:
    ValueType m_value;

public:
    explicit Bool(ValueType value) : m_value{ value } { }

    [[nodiscard]] std::string string_representation() const override {
        if (m_value) {
            return "true";
        } else {
            return "false";
        }
    }

    [[nodiscard]] char const* type_name() const override {
        return "Bool";
    }

    [[nodiscard]] Value clone() const override {
        return std::make_unique<Bool>(m_value);
    }

    [[nodiscard]] bool value() const {
        return m_value;
    }

    [[nodiscard]] bool is_bool_value() const override {
        return true;
    }

    [[nodiscard]] Bool const& as_bool_value() const override {
        return *this;
    }

    [[nodiscard]] Value logical_and(Value const& other) const override;

    [[nodiscard]] Value logical_or(Value const& other) const override;
};

class Iterator : public BasicValue {
public:
    [[nodiscard]] bool is_iterator() const final {
        return true;
    }

    [[nodiscard]] Iterator const& as_iterator() const override = 0;

    [[nodiscard]] virtual std::pair<Value, Value> next() const = 0;
};

class Sentinel : public BasicValue {
public:
    [[nodiscard]] bool is_sentinel() const override {
        return true;
    }

    [[nodiscard]] std::string string_representation() const override {
        return "Sentinel";
    }

    [[nodiscard]] char const* type_name() const override {
        return "Sentinel";
    }

    [[nodiscard]] Value clone() const override {
        return std::make_unique<Sentinel>();
    }
};

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
    RangeIterator(Value start, bool const end_is_inclusive, Value end)
        : RangeIterator{ start->clone(), end_is_inclusive, end->clone(), start->clone(), Key{} } { }

private:
    // for pass key idiom
    struct Key { };

public:
    RangeIterator(Value start, bool const end_is_inclusive, Value end, Value current, Key)
        : m_start{ std::move(start) },
          m_end_is_inclusive{ end_is_inclusive },
          m_end{ std::move(end) },
          m_current{ std::move(current) } {
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

public:
    [[nodiscard]] std::string string_representation() const override {
        return std::format("{}..{}", m_start->string_representation(), m_end->string_representation());
    }

    [[nodiscard]] char const* type_name() const override {
        return "Range";
    }

    [[nodiscard]] Value clone() const override {
        return std::make_unique<RangeIterator>(
                m_start->clone(),
                m_end_is_inclusive,
                m_end->clone(),
                m_current->clone(),
                Key{}
        );
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
            return { clone(), std::make_unique<Sentinel>() };
        }
        auto const new_value = m_current->as_integer_value().value() + (m_direction == Direction::Increasing ? 1 : -1);
        return { std::make_unique<RangeIterator>(
                         m_start->clone(),
                         m_end_is_inclusive,
                         m_end->clone(),
                         std::make_unique<Integer>(new_value),
                         Key{}
                 ),
                 std::make_unique<Integer>(current_value) };
    }
};

[[nodiscard]] inline Value make_integer_value(Integer::ValueType const value) {
    return std::make_unique<Integer>(value);
}

[[nodiscard]] inline Value make_string_value(String::ValueType value) {
    return std::make_unique<String>(std::move(value));
}

[[nodiscard]] inline Value make_bool_value(Bool::ValueType const value) {
    return std::make_unique<Bool>(value);
}
