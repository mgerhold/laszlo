#pragma once

#include "token.hpp"
#include "types.hpp"
#include <concepts>
#include <format>
#include <numeric>
#include <stdexcept>
#include <string_view>
#include <vector>

class Expression;

class RuntimeError : public std::exception {
private:
    std::string m_message;

protected:
    explicit RuntimeError(std::string message) : m_message{ std::move(message) } { }

public:
    [[nodiscard]] char const* what() const noexcept override {
        return m_message.c_str();
    }
};

class OperationNotSupportedByType : public RuntimeError {
public:
    template<std::convertible_to<types::Type>... Types>
    explicit OperationNotSupportedByType(std::string_view const operation, Types... types)
        : RuntimeError{ std::format(
                  "operation '{}' cannot be applied to type(s) '{}'",
                  operation,
                  join(", ", types->to_string()...)
          ) } { }

private:
    template<std::convertible_to<std::string_view> First, std::convertible_to<std::string_view>... Strings>
    [[nodiscard]] static std::string join(std::string_view const separator, First first, Strings... strings) {
        if constexpr (sizeof...(strings) > 0) {
            return std::format("{}, {}", first, join(separator, strings...));
        } else {
            return std::string{ first };
        }
    }
};

class IntegerOverflow : public RuntimeError {
public:
    explicit IntegerOverflow() : RuntimeError{ "integer overflow" } { }
};

class VariableRedefinition : public RuntimeError {
public:
    explicit VariableRedefinition(Token const token)
        : RuntimeError{ std::format("{}: redefinition of variable '{}'", token.source_location, token.lexeme()) } { }
};

class UndefinedReference : public RuntimeError {
public:
    explicit UndefinedReference(Token const token)
        : RuntimeError{ std::format("{}: undefined reference to name '{}'", token.source_location, token.lexeme()) } { }
};

class TypeMismatch : public RuntimeError {
public:
    TypeMismatch(SourceLocation const source_location, types::Type const& expected, types::Type const& actual)
        : RuntimeError{ std::format(
                  "{}: type mismatch (expected '{}', got '{}')",
                  source_location,
                  expected->to_string(),
                  actual->to_string()
          ) } { }
};

class ReturnTypeMismatch : public RuntimeError {
public:
    ReturnTypeMismatch(SourceLocation const source_location, types::Type const& expected, types::Type const& actual)
        : RuntimeError{ std::format(
                  "{}: returning value of wrong type from function (expected '{}', got '{}')",
                  source_location,
                  expected->to_string(),
                  actual->to_string()
          ) } { }
};

class FailedAssertion : public RuntimeError {
public:
    explicit FailedAssertion(SourceLocation const source_location)
        : RuntimeError{
              std::format("{}: assertion failed, '{}' did not hold true", source_location, source_location.text())
          } { }
};

class UnableToSubscript : public RuntimeError {
public:
    UnableToSubscript(types::Type const& index_type, types::Type const& expression_type)
        : RuntimeError{ std::format(
                  "expression of type '{}' cannot be used to subscript object of type '{}'",
                  index_type->to_string(),
                  expression_type->to_string()
          ) } { }
};

class IndexOutOfBounds : public RuntimeError {
public:
    IndexOutOfBounds(std::int32_t const index, std::int32_t const size)
        : RuntimeError{ std::format("index '{}' is out of bounds for array of size '{}'", index, size) } { }
};

class InvalidValueCast : public RuntimeError {
public:
    explicit InvalidValueCast(std::string_view const target_type)
        : RuntimeError{ std::format("unable to cast value to target type '{}'", target_type) } { }
};

class LvalueRequired : public RuntimeError {
public:
    explicit LvalueRequired() : RuntimeError{ std::format("got an rvalue where an lvalue is required") } { }
};

class WrongNumberOfArguments : public RuntimeError {
public:
    WrongNumberOfArguments(Token const function_name, std::size_t const expected_count, std::size_t const actual_count)
        : RuntimeError{ std::format(
                  "{}: wrong number of arguments when calling function '{}' (expected {}, got {})",
                  function_name.source_location,
                  function_name.lexeme(),
                  expected_count,
                  actual_count
          ) } { }
};

class WrongArgumentType : public RuntimeError {
public:
    WrongArgumentType(Token const parameter_name, types::Type const& expected, types::Type const& actual)
        : RuntimeError{ std::format(
                  "{}: wrong argument type (expected '{}', got '{}'",
                  parameter_name.source_location,
                  expected->to_string(),
                  actual->to_string()
          ) } { }
};
