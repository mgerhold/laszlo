#include "lexer.hpp"
#include "lexer_error.hpp"
#include "overloaded.hpp"
#include "source_location.hpp"
#include <format>
#include <stdexcept>

class LexerState final {
private:
    std::string_view m_filename;
    std::string_view m_source;
    std::size_t m_current_index{ 0 };

public:
    explicit LexerState(std::string_view const filename, std::string_view const source)
        : m_filename{ filename },
          m_source{ source } { }

    [[nodiscard]] bool is_at_end() const {
        return m_current_index >= m_source.length();
    }

    [[nodiscard]] char current() const {
        return is_at_end() ? '\0' : m_source.at(m_current_index);
    }

    [[nodiscard]] std::string_view substring(std::size_t const position, std::size_t const length) const {
        return m_source.substr(position, length);
    }

    [[nodiscard]] SourceLocation source_location(std::size_t const byte_offset, std::size_t const num_bytes) const {
        return SourceLocation{ m_filename, m_source, byte_offset, num_bytes };
    }

    [[nodiscard]] SourceLocation current_source_location(std::size_t const num_bytes = 1) const {
        return source_location(m_current_index, num_bytes);
    }

    void advance() {
        ++m_current_index;
    }

    friend class Tokens;
};

[[nodiscard]] static bool is_valid_identifier_start(char const c) {
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or c == '_';
}

[[nodiscard]] static bool is_valid_identifier_continuation(char const c) {
    return is_valid_identifier_start(c) or (c >= '0' and c <= '9');
}

[[nodiscard]] Tokens Tokens::tokenize(std::string_view const filename, std::string_view const source) {
    auto state = LexerState{ filename, source };
    auto tokens = Tokens{};

    auto const add_token = overloaded{
        [&](TokenType const type, std::size_t const position, std::size_t const length = 1) {
            auto const source_location = SourceLocation{ filename, source, position, length };
            tokens.m_tokens.emplace_back(type, source_location);
        },
        [&](TokenType const type) {
            auto const source_location = SourceLocation{ filename, source, state.m_current_index, 1 };
            tokens.m_tokens.emplace_back(type, source_location);
        },
    };

    while (not state.is_at_end()) {
        auto const current = state.current();
        switch (current) {
            case '(':
                add_token(TokenType::LeftParenthesis);
                state.advance();
                break;
            case ')':
                add_token(TokenType::RightParenthesis);
                state.advance();
                break;
            case ';':
                add_token(TokenType::Semicolon);
                state.advance();
                break;
            case '"': {
                auto const start = state.m_current_index;
                auto length = 1;
                state.advance();
                while (not state.is_at_end() and state.current() != '"') {
                    if (state.current() == '\n') {
                        throw LexerError{
                            ForbiddenCharacterInStringLiteral{ state.current_source_location(), state.current() }
                        };
                    }
                    state.advance();
                    ++length;
                }
                if (state.is_at_end()) {
                    throw LexerError{
                        UnclosedStringLiteral{ state.source_location(start, length),
                                              std::string{ state.substring(start, length) } }
                    };
                }
                state.advance();
                ++length;
                add_token(TokenType::StringLiteral, start, length);
                break;
            }
            default:
                if (std::isspace(static_cast<unsigned char>(current))) {
                    state.advance();
                    continue;
                }

                if (std::isdigit(static_cast<unsigned char>(current))) {
                    auto const start = state.m_current_index;
                    auto length = std::size_t{ 1 };
                    state.advance();

                    while (not state.is_at_end() and std::isdigit(static_cast<unsigned char>(state.current()))) {
                        ++length;
                        state.advance();
                    }
                    add_token(TokenType::IntegerLiteral, start, length);
                    continue;
                }

                if (is_valid_identifier_start(current)) {
                    auto const start = state.m_current_index;
                    auto length = std::size_t{ 1 };
                    state.advance();
                    while (not state.is_at_end() and is_valid_identifier_continuation(state.current())) {
                        ++length;
                        state.advance();
                    }
                    add_token(TokenType::Identifier, start, length);
                    continue;
                }

                throw LexerError{
                    UnexpectedChar{ state.current_source_location(), current }
                };
        }
    }

    tokens.m_tokens.emplace_back(TokenType::EndOfInput, SourceLocation{ filename, source, source.length() - 1, 1 });
    return tokens;
}
