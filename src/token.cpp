#include "token.hpp"
#include "runtime_error.hpp"

[[nodiscard]] std::int32_t Token::parse_integer() const {
    assert(type == TokenType::IntegerLiteral);
    auto stream = std::stringstream{};
    stream << lexeme();
    auto result = std::int32_t{};
    stream >> result;
    if (not stream) {
        throw InvalidIntegerValue{ lexeme() };
    }
    return result;
}
