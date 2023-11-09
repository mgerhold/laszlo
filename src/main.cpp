#include "interpreter.hpp"
#include "lexer.hpp"
#include "lexer_error.hpp"
#include "parser.hpp"
#include "parser_error.hpp"
#include "runtime_error.hpp"
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

[[nodiscard]] std::string read_file(std::filesystem::path const& path) {
    auto const file = std::ifstream{ path };
    if (not file) {
        throw std::runtime_error{ std::format("unable to open file '{}'", path.string()) };
    }
    auto stream = std::stringstream{};
    stream << file.rdbuf();
    return std::move(stream).str();
}

int main(int const argc, char const* const* const argv) try {
#ifdef EMSCRIPTEN
    auto const filename = std::string_view{ "programs/test.las" };
#else
    assert(argc >= 1);
    if (argc != 2) {
        std::cerr << std::format("error: no input file\nusage: {} <INPUT_FILENAME>\n", argv[0]);
        return EXIT_FAILURE;
    }
    auto const filename = std::string_view{ argv[1] };
#endif
    auto const source = read_file(filename);
    auto const tokens = Tokens::tokenize(filename, source);
    auto const ast = parse(tokens);
    interpret(ast);
} catch (LexerError const& error) {
    std::cerr << '\n' << error.what() << '\n';
} catch (ParserError const& error) {
    std::cerr << '\n' << error.what() << '\n';
} catch (RuntimeError const& error) {
    std::cerr << '\n' << error.what() << '\n';
} catch (ControlFlowException const& error) {
    std::cerr << '\n' << error.what() << '\n';
} catch (std::exception const& exception) {
    std::cerr << "\nunexpected error: " << exception.what() << '\n';
}
