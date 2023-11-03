#include "interpreter.hpp"
#include "lexer.hpp"
#include "lexer_error.hpp"
#include "parser.hpp"
#include "parser_error.hpp"
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

[[nodiscard]] std::string read_file(std::filesystem::path const& path) {
    auto file = std::ifstream{ path };
    if (not file) {
        throw std::runtime_error{ std::format("unable to open file '{}'", path.string()) };
    }
    auto stream = std::stringstream{};
    stream << file.rdbuf();
    return std::move(stream).str();
}

int main() try {
    auto filename = std::string_view{ "test.las" };
    auto const source = read_file(filename);
    auto const tokens = Tokens::tokenize(filename, source);
    auto const ast = parse(tokens);
    interpret(ast);
} catch (LexerError const& error) {
    std::cerr << error.what() << '\n';
} catch (ParserError const& error) {
    std::cerr << error.what() << '\n';
} catch (std::exception const& exception) {
    std::cerr << "unexpected error: " << exception.what() << '\n';
}
