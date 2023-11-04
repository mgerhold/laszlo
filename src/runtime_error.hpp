#pragma once

#include "token.hpp"
#include <concepts>
#include <format>
#include <numeric>
#include <stdexcept>
#include <vector>

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
    template<std::convertible_to<std::string_view>... Types>
    explicit OperationNotSupportedByType(std::string_view const operation, Types... types)
        : RuntimeError{
              std::format("operation '{}' cannot be applied to type(s) '{}'", operation, join(", ", types...))
          } { }

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
    TypeMismatch(SourceLocation const source_location, std::string_view const expected, std::string_view const actual)
        : RuntimeError{ std::format("{}: type mismatch: expected '{}', got '{}'", source_location, expected, actual) } {
    }
};

class FailedAssertion : public RuntimeError {
public:
    explicit FailedAssertion(SourceLocation const source_location)
        : RuntimeError{
              std::format("{}: assertion failed, '{}' did not hold true", source_location, source_location.text())
          } { }
};
