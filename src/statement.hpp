#pragma once

#include "expression.hpp"

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
