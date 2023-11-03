#pragma once

#include "token.hpp"
#include <variant>

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
