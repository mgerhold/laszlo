#pragma once

#include "lexer.hpp"
#include <sstream>
#include <variant>
#include <vector>

class StringLiteral {
private:
    Token m_token;

public:
    explicit StringLiteral(Token token) : m_token{ token } { }

    [[nodiscard]] Token const& token() const {
        return m_token;
    }
};

using Expression = std::variant<StringLiteral>;

class UnexpectedToken {
private:
    std::string m_message;

public:
    explicit UnexpectedToken(Token token) {
        auto stream = std::stringstream{};
        stream << "unexpected token '" << token << '\'';
        m_message = std::move(stream).str();
    }

    friend class ParserError;
};

using ParserErrorKind = std::variant<UnexpectedToken>;

class ParserError : public std::exception {
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

class Print {
private:
    Expression m_expression;

public:
    explicit Print(Expression expression) : m_expression{ expression } { }

    [[nodiscard]] Expression const& expression() const {
        return m_expression;
    }
};

using Statement = std::variant<Print>;

class Statements {
private:
    std::vector<Statement> m_statements;

public:
    explicit Statements(std::vector<Statement> statements) : m_statements{ std::move(statements) } { }

    [[nodiscard]] auto begin() const {
        return m_statements.begin();
    }

    [[nodiscard]] auto end() const {
        return m_statements.end();
    }
};

using AbstractSyntaxTree = Statements;

[[nodiscard]] AbstractSyntaxTree parse(Tokens const& tokens);
