#pragma once

#include "expression.hpp"

class Statement {
protected:
    Statement() = default;

public:
    virtual ~Statement() = default;

    virtual void execute() const = 0;
};

class Print final : public Statement {
private:
    std::unique_ptr<Expression> m_expression;

public:
    explicit Print(std::unique_ptr<Expression> expression) : m_expression{ std::move(expression) } { }

    void execute() const override {
        std::cout << m_expression->evaluate()->string_representation() << '\n';
    }
};

using Statements = std::vector<std::unique_ptr<Statement>>;
