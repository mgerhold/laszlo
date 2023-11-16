#pragma once

#include "source_location.hpp"
#include <variant>

class LexerErrorBase {
private:
    SourceLocation m_source_location;
    std::string m_message;

public:
    LexerErrorBase(SourceLocation const& source_location, std::string message)
        : m_message{ std::format("{}: {}", source_location, std::move(message)) },
          m_source_location{ source_location } { }

    [[nodiscard]] char const* message() const {
        return m_message.c_str();
    }

    [[nodiscard]] SourceLocation const& source_location() const {
        return m_source_location;
    }
};

class UnexpectedChar final : public LexerErrorBase {
public:
    UnexpectedChar(SourceLocation const& source_location, char const c)
        : LexerErrorBase{ source_location, std::format("unexpected char: '{}'", c) } { }
};

class UnclosedStringLiteral final : public LexerErrorBase {
public:
    UnclosedStringLiteral(SourceLocation const& source_location, std::string literal_beginning)
        : LexerErrorBase{ source_location, std::format("unclosed string literal: {}", literal_beginning) } { }
};

class UnclosedCharLiteral final : public LexerErrorBase {
public:
    explicit UnclosedCharLiteral(SourceLocation const& source_location)
        : LexerErrorBase{ source_location, "unclosed char literal" } { }
};

class InvalidEscapeSequence final : public LexerErrorBase {
public:
    explicit InvalidEscapeSequence(SourceLocation const& source_location)
        : LexerErrorBase{ source_location, "invalid escape sequence" } { }
};

class InvalidCharLiteral final : public LexerErrorBase {
public:
    explicit InvalidCharLiteral(SourceLocation const& source_location)
        : LexerErrorBase{ source_location, "invalid char literal" } { }
};

class ForbiddenCharacterInStringLiteral final : public LexerErrorBase {
public:
    ForbiddenCharacterInStringLiteral(SourceLocation const& source_location, char const c)
        : LexerErrorBase{ source_location,
                          std::format("character '{}' is not allowed inside a string literal", make_printable(c)) } { }

private:
    [[nodiscard]] static std::string make_printable(char const c) {
        if (c == '\n') {
            return "\\n";
        }
        return std::string{ c };
    }
};

using LexerErrorKind = std::variant<
        UnexpectedChar,
        UnclosedStringLiteral,
        ForbiddenCharacterInStringLiteral,
        UnclosedCharLiteral,
        InvalidEscapeSequence,
        InvalidCharLiteral>;

class LexerError final : public std::exception {
private:
    LexerErrorKind m_kind;

public:
    explicit LexerError(LexerErrorKind kind) : m_kind{ std::move(kind) } { }

    [[nodiscard]] char const* what() const noexcept override {
        return std::visit([](auto const& error) { return error.message(); }, m_kind);
    }
};
