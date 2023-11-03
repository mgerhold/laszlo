#pragma once

#include "expression.hpp"
#include "lexer.hpp"
#include "statement.hpp"
#include <sstream>
#include <variant>
#include <vector>

[[nodiscard]] Statements parse(Tokens const& tokens);
