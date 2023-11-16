#pragma once

#include "token.hpp"
#include <string>
#include <variant>

class UnexpectedToken final {
private:
    std::string m_message;

public:
    explicit UnexpectedToken(Token token) {
        m_message = std::format("{}: unexpected token '{}'", token.source_location, token);
    }

    friend class ParserError;
};

using ParserErrorKind = std::variant<UnexpectedToken>;

class ParserError final : public std::exception {
private:
    ParserErrorKind m_kind;

public:
    explicit ParserError(ParserErrorKind kind) : m_kind{ std::move(kind) } { }

    [[nodiscard]] char const* what() const noexcept override {
        return std::visit(
                [](UnexpectedToken const& unexpected_token) { return unexpected_token.m_message.c_str(); },
                m_kind
        );
    }
};
