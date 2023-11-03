#include "interpreter.hpp"
#include "overloaded.hpp"
#include <variant>

static std::string evaluate(Expression const& expression) {
    return std::visit(
            [](StringLiteral const& literal) {
                return std::string{ literal.token().lexeme().substr(1, literal.token().lexeme().length() - 2) };
            },
            expression
    );
}

static void interpret(Statement const& statement) {
    std::visit(
            overloaded{
                    [&](Print const& print) {
                        auto const& expression = print.expression();
                        auto const string = evaluate(expression);
                        std::cout << string << '\n';
                    },
            },
            statement
    );
}

void interpret(AbstractSyntaxTree const& ast) {
    for (auto const& statement : ast) {
        interpret(statement);
    }
}
