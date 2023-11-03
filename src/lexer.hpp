#pragma once

#include <string_view>
#include <vector>
#include <iostream>
#include "token.hpp"

class Tokens {
private:
    std::vector<Token> m_tokens;

public:
    operator const std::vector<Token>&() const { // NOLINT
        return m_tokens;
    }

    [[nodiscard]] static Tokens tokenize(std::string_view filename, std::string_view source);

    [[nodiscard]] std::size_t size() const {
        return m_tokens.size();
    }

    [[nodiscard]] Token const& operator[](std::size_t const index) const {
        return m_tokens.at(index);
    }

    [[nodiscard]] Token& operator[](std::size_t const index) {
        return m_tokens.at(index);
    }

    [[nodiscard]] Token const& back() const {
        return m_tokens.back();
    }

    [[nodiscard]] Token& back() {
        return m_tokens.back();
    }

    friend std::ostream& operator<<(std::ostream& os, Tokens const& tokens) {
        for (const auto& token : tokens.m_tokens) {
            os << token << '\n';
        }
        return os;
    }
};