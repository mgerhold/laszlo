#include "parser.hpp"

class ParserState final {
private:
    Tokens const& m_tokens;
    std::size_t m_current_index{ 0 };

public:
    explicit ParserState(Tokens const& tokens) : m_tokens{ tokens } { }

    [[nodiscard]] bool is_at_end() const {
        return m_current_index >= m_tokens.size() or m_tokens[m_current_index].type == TokenType::EndOfInput;
    }

    [[nodiscard]] Token const& current() const {
        return (is_at_end() ? m_tokens.back() : m_tokens[m_current_index]);
    }

    [[nodiscard]] Token const& peek() const {
        if (m_current_index + 1 >= m_tokens.size()) {
            return m_tokens.back();
        }
        return m_tokens[m_current_index + 1];
    }

    void advance() {
        ++m_current_index;
    }

    [[nodiscard]] std::vector<Statement> statements() {
        auto statements = std::vector<Statement>{};

        while (not is_at_end()) {
            statements.push_back(statement());
        }

        return statements;
    }

    [[nodiscard]] Expression expression() {
        switch (current().type) {
            case TokenType::StringLiteral: {
                auto literal = current();
                advance();
                return StringLiteral{ literal };
            }
            default:
                throw ParserError{ UnexpectedToken{ current() } };
        }
    }

    void expect(TokenType type) {
        if (current().type != type) {
            throw ParserError{ UnexpectedToken{ current() } };
        }
        advance();
    }

    [[nodiscard]] Statement statement() {
        using enum TokenType;
        switch (current().type) {
            case Identifier:
                if (current().lexeme() == "print") {
                    advance(); // consume "print"
                    expect(TokenType::LeftParenthesis);
                    auto expr = expression();
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return Print{ expr };
                }
            default:
                throw ParserError{ UnexpectedToken{ current() } };
        }
    }
};

[[nodiscard]] AbstractSyntaxTree parse(Tokens const& tokens) {
    auto state = ParserState{ tokens };
    return AbstractSyntaxTree{ state.statements() };
}
