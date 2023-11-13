#pragma once

#include "source_location.hpp"
#include <cassert>
#include <iostream>
#include <string_view>

enum class TokenType {
    LeftParenthesis,
    RightParenthesis,
    Semicolon,
    Comma,
    StringLiteral,
    IntegerLiteral,
    CharLiteral,
    Identifier,
    Plus,
    Minus,
    Asterisk,
    Slash,
    Equals,
    EqualsGreaterThan,
    PlusEquals,
    MinusEquals,
    AsteriskEquals,
    SlashEquals,
    LeftCurlyBracket,
    RightCurlyBracket,
    LeftSquareBracket,
    RightSquareBracket,
    GreaterThan,
    GreaterOrEqual,
    LessThan,
    LessOrEqual,
    EqualsEquals,
    ExclamationMarkEquals,
    ExclamationMark,
    Dot,
    DotDot,
    Colon,
    TildeArrow,
    QuestionMark,
    EndOfInput,
};

struct Token final {
    TokenType type;
    SourceLocation source_location;

    Token(TokenType const type, SourceLocation const& source_location)
        : type{ type },
          source_location{ source_location } { }

    [[nodiscard]] std::string_view lexeme() const {
        return source_location.text();
    }

    [[nodiscard]] std::int32_t parse_integer() const;

    friend std::ostream& operator<<(std::ostream& os, Token const& token) {
        switch (token.type) {
            default:
                assert(false and "unreachable");
                return os;
            case TokenType::LeftParenthesis:
                return os << "LeftParenthesis";
            case TokenType::RightParenthesis:
                return os << "RightParenthesis";
            case TokenType::Semicolon:
                return os << "Semicolon";
            case TokenType::Comma:
                return os << "Comma";
            case TokenType::StringLiteral:
                return os << "StringLiteral(" << token.lexeme() << ')';
            case TokenType::IntegerLiteral:
                return os << "IntegerLiteral(" << token.lexeme() << ')';
            case TokenType::Identifier:
                return os << "Identifier(" << token.lexeme() << ')';
            case TokenType::EndOfInput:
                return os << "EndOfInput";
            case TokenType::Plus:
                return os << "Plus";
            case TokenType::Minus:
                return os << "Minus";
            case TokenType::Equals:
                return os << "Equals";
            case TokenType::LeftCurlyBracket:
                return os << "LeftCurlyBracket";
            case TokenType::RightCurlyBracket:
                return os << "RightCurlyBracket";
            case TokenType::GreaterThan:
                return os << "GreaterThan";
            case TokenType::GreaterOrEqual:
                return os << "GreaterOrEqual";
            case TokenType::LessThan:
                return os << "LessThan";
            case TokenType::LessOrEqual:
                return os << "LessOrEqual";
            case TokenType::EqualsEquals:
                return os << "EqualsEquals";
            case TokenType::Asterisk:
                return os << "Asterisk";
            case TokenType::Slash:
                return os << "Slash";
            case TokenType::LeftSquareBracket:
                return os << "LeftSquareBracket";
            case TokenType::RightSquareBracket:
                return os << "RightSquareBracket";
            case TokenType::ExclamationMarkEquals:
                return os << "ExclamationMarkEquals";
            case TokenType::ExclamationMark:
                return os << "ExclamationMark";
            case TokenType::Dot:
                return os << "Dot";
            case TokenType::DotDot:
                return os << "DotDot";
            case TokenType::Colon:
                return os << "Colon";
            case TokenType::TildeArrow:
                return os << "TildeArray";
            case TokenType::CharLiteral:
                return os << "CharLiteral";
            case TokenType::PlusEquals:
                return os << "PlusEquals";
            case TokenType::MinusEquals:
                return os << "MinusEquals";
            case TokenType::AsteriskEquals:
                return os << "AsteriskEquals";
            case TokenType::SlashEquals:
                return os << "SlashEquals";
            case TokenType::QuestionMark:
                return os << "QuestionMark";
        }
    }
};

template<>
struct std::formatter<Token> : ostream_formatter { };
