#pragma once

#include "parser.hpp"

inline void interpret(Statements const& program) {
    for (auto const& statement : program) {
        statement->execute();
    }
}