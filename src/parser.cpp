#include "parser.hpp"
#include "parser_error.hpp"

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

    Token advance() {
        return m_tokens[m_current_index++];
    }

    [[nodiscard]] std::vector<std::unique_ptr<Statement>> statements() {
        auto statements = Statements{};

        while (not is_at_end()) {
            statements.push_back(statement());
        }

        return statements;
    }

    [[nodiscard]] std::unique_ptr<Expression> expression() {
        return range();
    }

    [[nodiscard]] std::unique_ptr<Expression> range() {
        auto start = logical_or();
        if (current().type == TokenType::DotDot) {
            advance(); // consume ".."
            auto const end_is_inclusive = (current().type == TokenType::Equals);
            if (end_is_inclusive) {
                advance(); // consume "="
            }
            auto end = logical_or();
            return std::make_unique<Range>(std::move(start), end_is_inclusive, std::move(end));
        }
        return start;
    }

    [[nodiscard]] std::unique_ptr<Expression> logical_or() {
        auto accumulator = logical_and();
        while (current().type == TokenType::Identifier and current().lexeme() == "or") {
            advance(); // consume "or"
            accumulator =
                    std::make_unique<BinaryOperator>(std::move(accumulator), BinaryOperator::Kind::Or, logical_and());
        }
        return accumulator;
    }

    [[nodiscard]] std::unique_ptr<Expression> logical_and() {
        auto accumulator = equals_or_unequals();
        while (current().type == TokenType::Identifier and current().lexeme() == "and") {
            advance(); // consume "and"
            accumulator = std::make_unique<BinaryOperator>(
                    std::move(accumulator),
                    BinaryOperator::Kind::And,
                    equals_or_unequals()
            );
        }
        return accumulator;
    }

    [[nodiscard]] std::unique_ptr<Expression> equals_or_unequals() {
        auto accumulator = relational_operator();
        while (true) {
            switch (current().type) {
                case TokenType::EqualsEquals:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::Equality,
                            relational_operator()
                    );
                    break;
                case TokenType::ExclamationMarkEquals:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::Inequality,
                            relational_operator()
                    );
                    break;
                default:
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<Expression> relational_operator() {
        auto accumulator = sum();
        while (true) {
            switch (current().type) {
                case TokenType::LessThan:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::LessThan,
                            sum()
                    );
                    break;
                case TokenType::LessOrEqual:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::LessOrEqual,
                            sum()
                    );
                    break;
                case TokenType::GreaterThan:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::GreaterThan,
                            sum()
                    );
                    break;
                case TokenType::GreaterOrEqual:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::GreaterOrEqual,
                            sum()
                    );
                    break;
                default:
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<Expression> sum() {
        auto accumulator = product();
        while (true) {
            switch (current().type) {
                case TokenType::Plus:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::Plus,
                            product()
                    );
                    break;
                case TokenType::Minus:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::Minus,
                            product()
                    );
                    break;
                default:
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<Expression> product() {
        auto accumulator = unary_operator();
        while (true) {
            switch (current().type) {
                case TokenType::Asterisk:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::Multiply,
                            unary_operator()
                    );
                    break;
                case TokenType::Slash:
                    advance();
                    accumulator = std::make_unique<BinaryOperator>(
                            std::move(accumulator),
                            BinaryOperator::Kind::Divide,
                            unary_operator()
                    );
                    break;
                default:
                    if (current().type == TokenType::Identifier and current().lexeme() == "mod") {
                        advance(); // consume "mod"
                        accumulator = std::make_unique<BinaryOperator>(
                                std::move(accumulator),
                                BinaryOperator::Kind::Mod,
                                unary_operator()
                        );
                        break;
                    }
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<Expression> unary_operator() {
        switch (current().type) {
            case TokenType::Plus:
                return std::make_unique<UnaryOperator>(advance(), expression());
            case TokenType::Minus:
                return std::make_unique<UnaryOperator>(advance(), expression());
            default:
                return primary();
        }
    }

    [[nodiscard]] std::unique_ptr<Expression> primary() {
        switch (current().type) {
            case TokenType::StringLiteral:
                return std::make_unique<StringLiteral>(advance());
            case TokenType::IntegerLiteral: {
                return std::make_unique<IntegerLiteral>(advance());
            }
            case TokenType::LeftParenthesis: {
                advance();
                auto expr = expression();
                expect(TokenType::RightParenthesis);
                return expr;
            }
            case TokenType::Identifier: {
                auto token = advance();
                if (token.lexeme() == "true" or token.lexeme() == "false") {
                    return std::make_unique<BoolLiteral>(token);
                }
                return std::make_unique<Name>(token);
            }
            default:
                throw ParserError{ UnexpectedToken{ current() } };
        }
    }

    Token expect(TokenType type) {
        if (current().type != type) {
            throw ParserError{ UnexpectedToken{ current() } };
        }
        return advance();
    }

    [[nodiscard]] std::unique_ptr<Statement> block() {
        auto statements = Statements{};
        expect(TokenType::LeftCurlyBracket);
        while (current().type != TokenType::RightCurlyBracket) {
            statements.push_back(statement());
        }
        advance(); // consume '}'
        return std::make_unique<Block>(std::move(statements));
    }

    [[nodiscard]] std::unique_ptr<Statement> statement() {
        using enum TokenType;
        switch (current().type) {
            case Identifier:
                if (current().lexeme() == "print") {
                    advance(); // consume "print"
                    expect(TokenType::LeftParenthesis);
                    // clang-format off
                    auto expr = std::unique_ptr<Expression>{
                            current().type == TokenType::RightParenthesis ? nullptr : expression()
                        };
                    // clang-format on
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return std::make_unique<Print>(std::move(expr));
                } else if (current().lexeme() == "println") {
                    advance(); // consume "print"
                    expect(TokenType::LeftParenthesis);
                    auto expr = std::unique_ptr<Expression>{
                        current().type == TokenType::RightParenthesis ? nullptr : expression()
                    };
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return std::make_unique<Println>(std::move(expr));
                } else if (current().lexeme() == "let") {
                    advance(); // consume "let"
                    auto name = expect(TokenType::Identifier);
                    expect(TokenType::Equals);
                    auto initializer = expression();
                    expect(TokenType::Semicolon);
                    return std::make_unique<VariableDefinition>(name, std::move(initializer));
                } else if (current().lexeme() == "if") {
                    return if_();
                } else if (current().lexeme() == "assert") {
                    advance(); // consume "assert"
                    expect(TokenType::LeftParenthesis);
                    auto predicate = expression();
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return std::make_unique<Assert>(std::move(predicate));
                } else if (current().lexeme() == "while") {
                    advance(); // consume "while"
                    auto condition = expression();
                    auto body = block();
                    return std::make_unique<While>(std::move(condition), std::move(body));
                } else if (current().lexeme() == "break") {
                    auto const break_token = advance();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Break>(break_token);
                } else if (current().lexeme() == "continue") {
                    auto const continue_token = advance();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Continue>(continue_token);
                } else if (current().lexeme() == "for") {
                    advance(); // consume "for"
                    auto const loop_variable = expect(TokenType::Identifier);
                    if (current().type != TokenType::Identifier or current().lexeme() != "in") {
                        throw ParserError{ UnexpectedToken{ current() } };
                    }
                    advance(); // consume "in"
                    auto iterator = expression();
                    auto body = block();
                    return std::make_unique<For>(loop_variable, std::move(iterator), std::move(body));
                } else {
                    auto lvalue = advance();
                    expect(TokenType::Equals);
                    auto value = expression();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Assignment>(lvalue, std::move(value));
                }
            case LeftCurlyBracket:
                return block();
            default:
                throw ParserError{ UnexpectedToken{ current() } };
        }
    }

    [[nodiscard]] std::unique_ptr<Statement> if_() {
        assert(current().type == TokenType::Identifier and current().lexeme() == "if");
        auto const if_token = advance(); // consume "if"
        auto condition = expression();
        auto then = block();
        if (current().type == TokenType::Identifier and current().lexeme() == "else") {
            advance(); // consume "else"
            if (current().type == TokenType::Identifier and current().lexeme() == "if") {
                return std::make_unique<If>(if_token, std::move(condition), std::move(then), if_());
            } else {
                return std::make_unique<If>(if_token, std::move(condition), std::move(then), block());
            }
        }
        return std::make_unique<If>(
                if_token,
                std::move(condition),
                std::move(then),
                std::make_unique<Block>(Statements{})
        );
    }
};

[[nodiscard]] Statements parse(Tokens const& tokens) {
    auto state = ParserState{ tokens };
    return Statements{ state.statements() };
}
