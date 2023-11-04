#pragma once

#include "token.hpp"
#include <format>
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
