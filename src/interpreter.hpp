#pragma once

#include "parser.hpp"

inline void interpret(statements::Statements const& program) {
    auto scope_stack = ScopeStack{};
    for (auto const& statement : program) {
        statement->execute(scope_stack);
    }
}
