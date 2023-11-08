#pragma once

#include "token.hpp"
#include "values/value.hpp"
#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

class ControlFlowException : public std::exception {
private:
    std::string m_message;

protected:
    explicit ControlFlowException(std::string message) : m_message{ std::move(message) } { }

public:
    [[nodiscard]] char const* what() const noexcept override {
        return m_message.c_str();
    }
};

class BreakException : public ControlFlowException {
public:
    explicit BreakException(Token const break_token)
        : ControlFlowException{ std::format("{}: usage of 'break' outside of loop", break_token.source_location) } { }
};

class ContinueException : public ControlFlowException {
public:
    explicit ContinueException(Token const continue_token)
        : ControlFlowException{
              std::format("{}: usage of 'continue' outside of loop", continue_token.source_location)
          } { }
};

class ReturnException : public ControlFlowException {
private:
    std::optional<values::Value> m_value;

    ReturnException(Token const return_token, std::optional<values::Value> value)
        : ControlFlowException{ std::format("{}: return statement outside of function", return_token.source_location) },
          m_value{ std::move(value) } { }

public:
    ReturnException(Token const return_token, values::Value value)
        : ReturnException{ return_token, std::optional<values::Value>{ std::move(value) } } { }

    explicit ReturnException(Token const return_token) : ReturnException{ return_token, std::nullopt } { }

    [[nodiscard]] std::optional<values::Value> value() const {
        return m_value.and_then([](auto const& value) { return std::optional{ value->clone() }; });
    }
};
