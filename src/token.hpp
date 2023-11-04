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
    IntegerLiteral,
    Identifier,
    Plus,
    Minus,
    Equals,
    LeftCurlyBracket,
    RightCurlyBracket,
    GreaterThan,
    GreaterOrEqual,
    LessThan,
    LessOrEqual,
    EqualsEquals,
    ExclamationMarkEquals,
    ExclamationMark,
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
                return os << "LeftParenthesis";
            case RightParenthesis:
                return os << "RightParenthesis";
            case Semicolon:
                return os << "Semicolon";
            case StringLiteral:
                return os << "StringLiteral(" << token.lexeme() << ')';
            case IntegerLiteral:
                return os << "IntegerLiteral(" << token.lexeme() << ')';
            case Identifier:
                return os << "Identifier(" << token.lexeme() << ')';
            case EndOfInput:
                return os << "EndOfInput";
            case Plus:
                return os << "Plus";
            case Minus:
                return os << "Minus";
            case Equals:
                return os << "Equals";
            case LeftCurlyBracket:
                return os << "LeftCurlyBracket";
            case RightCurlyBracket:
                return os << "RightCurlyBracket";
            case GreaterThan:
                return os << "GreaterThan";
            case GreaterOrEqual:
                return os << "GreaterOrEqual";
            case LessThan:
                return os << "LessThan";
            case LessOrEqual:
                return os << "LessOrEqual";
            case EqualsEquals:
                return os << "EqualsEquals";
            default:
                assert(false and "unreachable");
                return os;
        }
    }
};
