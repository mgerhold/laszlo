#include "parser.hpp"
#include "expressions/array_literal.hpp"
#include "expressions/binary_operator.hpp"
#include "expressions/bool_literal.hpp"
#include "expressions/call.hpp"
#include "expressions/integer_literal.hpp"
#include "expressions/member_access.hpp"
#include "expressions/name.hpp"
#include "expressions/range.hpp"
#include "expressions/string_literal.hpp"
#include "expressions/subscript.hpp"
#include "expressions/typeof.hpp"
#include "expressions/unary_operator.hpp"
#include "expressions/char_literal.hpp"
#include "parser_error.hpp"
#include "types.hpp"

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

    [[nodiscard]] std::unique_ptr<expressions::Expression> expression() { // NOLINT(misc-no-recursion)
        return range();
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> range() { // NOLINT(misc-no-recursion)
        auto start = logical_or();
        if (current().type == TokenType::DotDot) {
            advance(); // consume ".."
            auto const end_is_inclusive = (current().type == TokenType::Equals);
            if (end_is_inclusive) {
                advance(); // consume "="
            }
            auto end = logical_or();
            return std::make_unique<expressions::Range>(std::move(start), end_is_inclusive, std::move(end));
        }
        return start;
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> logical_or() { // NOLINT(misc-no-recursion)
        auto accumulator = logical_and();
        while (current().type == TokenType::Identifier and current().lexeme() == "or") {
            advance(); // consume "or"
            accumulator = std::make_unique<expressions::BinaryOperator>(
                    std::move(accumulator),
                    expressions::BinaryOperator::Kind::Or,
                    logical_and()
            );
        }
        return accumulator;
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> logical_and() { // NOLINT(misc-no-recursion)
        auto accumulator = equals_or_unequals();
        while (current().type == TokenType::Identifier and current().lexeme() == "and") {
            advance(); // consume "and"
            accumulator = std::make_unique<expressions::BinaryOperator>(
                    std::move(accumulator),
                    expressions::BinaryOperator::Kind::And,
                    equals_or_unequals()
            );
        }
        return accumulator;
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> equals_or_unequals() { // NOLINT(misc-no-recursion)
        auto accumulator = relational_operator();
        while (true) {
            switch (current().type) {
                case TokenType::EqualsEquals:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::Equality,
                            relational_operator()
                    );
                    break;
                case TokenType::ExclamationMarkEquals:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::Inequality,
                            relational_operator()
                    );
                    break;
                default:
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> relational_operator() { // NOLINT(misc-no-recursion)
        auto accumulator = sum();
        while (true) {
            switch (current().type) {
                case TokenType::LessThan:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::LessThan,
                            sum()
                    );
                    break;
                case TokenType::LessOrEqual:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::LessOrEqual,
                            sum()
                    );
                    break;
                case TokenType::GreaterThan:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::GreaterThan,
                            sum()
                    );
                    break;
                case TokenType::GreaterOrEqual:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::GreaterOrEqual,
                            sum()
                    );
                    break;
                default:
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> sum() { // NOLINT(misc-no-recursion)
        auto accumulator = product();
        while (true) {
            switch (current().type) {
                case TokenType::Plus:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::Plus,
                            product()
                    );
                    break;
                case TokenType::Minus:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::Minus,
                            product()
                    );
                    break;
                default:
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> product() { // NOLINT(misc-no-recursion)
        auto accumulator = unary_operator();
        while (true) {
            switch (current().type) {
                case TokenType::Asterisk:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::Multiply,
                            unary_operator()
                    );
                    break;
                case TokenType::Slash:
                    advance();
                    accumulator = std::make_unique<expressions::BinaryOperator>(
                            std::move(accumulator),
                            expressions::BinaryOperator::Kind::Divide,
                            unary_operator()
                    );
                    break;
                default:
                    if (current().type == TokenType::Identifier and current().lexeme() == "mod") {
                        advance(); // consume "mod"
                        accumulator = std::make_unique<expressions::BinaryOperator>(
                                std::move(accumulator),
                                expressions::BinaryOperator::Kind::Mod,
                                unary_operator()
                        );
                        break;
                    }
                    return accumulator;
            }
        }
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> unary_operator() { // NOLINT(misc-no-recursion)
        switch (current().type) {
            case TokenType::Plus:
            case TokenType::Minus: {
                auto const operator_token = advance();
                return std::make_unique<expressions::UnaryOperator>(operator_token, postfix_operator());
            }
            default:
                return postfix_operator();
        }
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> postfix_operator() { // NOLINT(misc-no-recursion)
        auto lvalue = primary();
        switch (current().type) {
            case TokenType::LeftSquareBracket: {
                advance(); // consume "["
                auto index = expression();
                auto const closing_bracket = expect(TokenType::RightSquareBracket);
                return std::make_unique<expressions::Subscript>(std::move(lvalue), std::move(index), closing_bracket);
            }
            case TokenType::LeftParenthesis: {
                advance(); // consume "("
                auto arguments = expression_list(TokenType::RightParenthesis);
                auto const closing_parenthesis = expect(TokenType::RightParenthesis);
                return std::make_unique<expressions::Call>(
                        std::move(lvalue),
                        std::move(arguments),
                        closing_parenthesis
                );
            }
            case TokenType::Dot: {
                advance(); // consume "."
                auto const member = expect(TokenType::Identifier);
                return std::make_unique<expressions::MemberAccess>(std::move(lvalue), member);
            }
            default:
                return lvalue;
        }
    }

    [[nodiscard]] std::unique_ptr<expressions::Expression> primary() { // NOLINT(misc-no-recursion)
        switch (current().type) {
            case TokenType::StringLiteral:
                return std::make_unique<expressions::StringLiteral>(advance());
            case TokenType::IntegerLiteral:
                return std::make_unique<expressions::IntegerLiteral>(advance());
            case TokenType::CharLiteral:
                return std::make_unique<expressions::CharLiteral>(advance());
            case TokenType::LeftSquareBracket: {
                auto const opening_bracket = advance(); // consume "["
                auto values = expression_list(TokenType::RightSquareBracket);
                auto const closing_bracket = advance(); // consume "]"
                return std::make_unique<expressions::ArrayLiteral>(opening_bracket, std::move(values), closing_bracket);
            }
            case TokenType::LeftParenthesis: {
                advance(); // consume "("
                auto expr = expression();
                expect(TokenType::RightParenthesis);
                return expr;
            }
            case TokenType::Identifier: {
                auto const token = advance();
                if (token.lexeme() == "true" or token.lexeme() == "false") {
                    return std::make_unique<expressions::BoolLiteral>(token);
                } else if (token.lexeme() == "typeof") {
                    expect(TokenType::LeftParenthesis);
                    auto expr = expression();
                    auto const closing_parenthesis = expect(TokenType::RightParenthesis);
                    return std::make_unique<expressions::TypeOf>(token, std::move(expr), closing_parenthesis);
                }
                return std::make_unique<expressions::Name>(token);
            }
            default:
                throw ParserError{ UnexpectedToken{ current() } };
        }
    }

    [[nodiscard]] std::vector<std::unique_ptr<expressions::Expression>> expression_list( // NOLINT(misc-no-recursion)
            TokenType const terminating_token
    ) {
        auto expressions = std::vector<std::unique_ptr<expressions::Expression>>{};
        while (not is_at_end() and current().type != terminating_token) {
            expressions.push_back(expression());
            if (current().type != TokenType::Comma) {
                break;
            }
            advance(); // consume ","
        }
        return expressions;
    }

    Token expect(TokenType type) {
        if (current().type != type) {
            throw ParserError{ UnexpectedToken{ current() } };
        }
        return advance();
    }

    [[nodiscard]] std::unique_ptr<Statement> block() { // NOLINT(misc-no-recursion)
        auto statements = Statements{};
        expect(TokenType::LeftCurlyBracket);
        while (current().type != TokenType::RightCurlyBracket) {
            statements.push_back(statement());
        }
        advance(); // consume '}'
        return std::make_unique<Block>(std::move(statements));
    }

    [[nodiscard]] std::unique_ptr<Statement> statement() { // NOLINT(misc-no-recursion)
        switch (current().type) {
            case TokenType::LeftCurlyBracket:
                return block();
            case TokenType::Identifier:
                if (current().lexeme() == "function") {
                    advance(); // consume "function"
                    auto const name = expect(TokenType::Identifier);
                    expect(TokenType::LeftParenthesis);
                    auto parameters = parameter_list();
                    expect(TokenType::RightParenthesis);
                    auto return_type = types::make_nothing();
                    if (current().type == TokenType::TildeArrow) {
                        advance(); // consume "~>"
                        return_type = data_type();
                    }
                    auto body = block();
                    return std::make_unique<FunctionDeclaration>(
                            name,
                            std::move(parameters),
                            std::move(return_type),
                            std::move(body)
                    );
                }
                if (current().lexeme() == "print") {
                    advance(); // consume "print"
                    expect(TokenType::LeftParenthesis);
                    // clang-format off
                    auto expr = std::unique_ptr<expressions::Expression>{
                            current().type == TokenType::RightParenthesis ? nullptr : expression()
                        };
                    // clang-format on
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return std::make_unique<Print>(std::move(expr));
                }
                if (current().lexeme() == "println") {
                    advance(); // consume "print"
                    expect(TokenType::LeftParenthesis);
                    // clang-format off
                    auto expr = std::unique_ptr<expressions::Expression>{
                            current().type == TokenType::RightParenthesis ? nullptr : expression()
                        };
                    // clang-format on
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return std::make_unique<Println>(std::move(expr));
                }
                if (current().lexeme() == "let") {
                    advance(); // consume "let"
                    auto name = expect(TokenType::Identifier);
                    expect(TokenType::Equals);
                    auto initializer = expression();
                    expect(TokenType::Semicolon);
                    return std::make_unique<VariableDefinition>(name, std::move(initializer));
                }
                if (current().lexeme() == "if") {
                    return if_();
                }
                if (current().lexeme() == "assert") {
                    advance(); // consume "assert"
                    expect(TokenType::LeftParenthesis);
                    auto predicate = expression();
                    expect(TokenType::RightParenthesis);
                    expect(TokenType::Semicolon);
                    return std::make_unique<Assert>(std::move(predicate));
                }
                if (current().lexeme() == "while") {
                    advance(); // consume "while"
                    auto condition = expression();
                    auto body = block();
                    return std::make_unique<While>(std::move(condition), std::move(body));
                }
                if (current().lexeme() == "break") {
                    auto const break_token = advance();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Break>(break_token);
                }
                if (current().lexeme() == "continue") {
                    auto const continue_token = advance();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Continue>(continue_token);
                }
                if (current().lexeme() == "return") {
                    auto const return_token = advance();
                    if (current().type == TokenType::Semicolon) {
                        advance(); // consume ";"
                        return std::make_unique<Return>(return_token, std::nullopt);
                    }
                    auto value = expression();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Return>(return_token, std::move(value));
                }
                if (current().lexeme() == "for") {
                    advance(); // consume "for"
                    auto const loop_variable = expect(TokenType::Identifier);
                    if (current().type != TokenType::Identifier or current().lexeme() != "in") {
                        throw ParserError{ UnexpectedToken{ current() } };
                    }
                    advance(); // consume "in"
                    auto iterator = expression();
                    auto body = block();
                    return std::make_unique<For>(loop_variable, std::move(iterator), std::move(body));
                }
                [[fallthrough]];
            default: {
                auto expr = expression();
                if (current().type == TokenType::Equals) {
                    // assignment
                    advance(); // consume "="
                    auto rvalue = expression();
                    expect(TokenType::Semicolon);
                    return std::make_unique<Assignment>(std::move(expr), std::move(rvalue));
                }
                // expression statement
                expect(TokenType::Semicolon);
                return std::make_unique<ExpressionStatement>(std::move(expr));
            }
        }
    }

    [[nodiscard]] std::vector<FunctionParameter> parameter_list() {
        auto parameters = std::vector<FunctionParameter>{};
        while (current().type == TokenType::Identifier) {
            auto const name = advance();
            expect(TokenType::Colon);
            auto type = data_type();
            parameters.emplace_back(name, std::move(type));
            if (current().type != TokenType::Comma) {
                break;
            }
            advance(); // consume ","
        }
        return parameters;
    }

    [[nodiscard]] std::unique_ptr<Statement> if_() { // NOLINT(misc-no-recursion)
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

    [[nodiscard]] types::Type data_type() {
        switch (current().type) {
            case TokenType::LeftSquareBracket: {
                advance(); // consume "["
                auto contained_type = data_type();
                expect(TokenType::RightSquareBracket);
                return types::make_array(std::move(contained_type));
            }
            case TokenType::QuestionMark:
                advance(); // consume "?"
                return types::make_unspecified();
            case TokenType::Identifier:
                if (current().lexeme() == "I32") {
                    advance();
                    return types::make_i32();
                } else if (current().lexeme() == "String") {
                    advance();
                    return types::make_string();
                } else if (current().lexeme() == "Bool") {
                    advance();
                    return types::make_bool();
                } else if (current().lexeme() == "Nothing") {
                    advance();
                    return types::make_nothing();
                } else if (current().lexeme() == "Range") {
                    advance();
                    return types::make_range();
                }
                [[fallthrough]];
            default:
                throw ParserError{ UnexpectedToken{ current() } };
        }
    }
};

[[nodiscard]] Statements parse(Tokens const& tokens) {
    auto state = ParserState{ tokens };
    return Statements{ state.statements() };
}
