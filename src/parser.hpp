#pragma once

#include "expressions/expression.hpp"
#include "lexer.hpp"
#include "statements/statement.hpp"
#include <sstream>
#include <variant>
#include <vector>

[[nodiscard]] statements::Statements parse(Tokens const& tokens);
