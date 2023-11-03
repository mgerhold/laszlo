#pragma once

#include "source_location.hpp"
#include <cassert>
#include <iostream>
#include <string_view>

enum class TokenType {
    LeftParenthesis,
    RightParenthesis,
    Semicolon,
    StringLiteral,
    Identifier,
    EndOfInput,
};

struct Token final {
    TokenType type;
    SourceLocation source_location;

    Token(TokenType type, SourceLocation source_location) : type{ type }, source_location{ source_location } { }

    [[nodiscard]] std::string_view lexeme() const {
        return source_location.text();
    }

    friend std::ostream& operator<<(std::ostream& os, Token token) {
        using enum TokenType;
        switch (token.type) {
            case LeftParenthesis:
                os << "LeftParenthesis";
                break;
            case RightParenthesis:
                os << "RightParenthesis";
                break;
            case Semicolon:
                os << "Semicolon";
                break;
            case StringLiteral:
                os << "StringLiteral(" << token.lexeme() << ")";
                break;
            case Identifier:
                os << "Identifier(" << token.lexeme() << ')';
                break;
            case EndOfInput:
                os << "EndOfInput";
                break;
            default:
                assert(false and "unreachable");
                break;
        }
        return os;
    }
};
