add_executable(
        laszlo
        main.cpp
        lexer.hpp
        lexer.cpp
        token.hpp
        overloaded.hpp
        parser.cpp
        parser.hpp
        interpreter.hpp
        source_location.hpp
        ostream_formatter.hpp
        parser_error.hpp
        lexer_error.hpp
        statement.hpp
        runtime_error.hpp
        scope.hpp
        control_flow.hpp
        values/value.hpp
        values/integer.hpp
        values/string.hpp
        values/bool.hpp
        values/Array.hpp
        values/iterator.hpp
        values/sentinel.hpp
        values/range_interator.hpp
        values/integer.cpp
        values/array_iterator.hpp
        values/range.hpp
        values/array.cpp
        expressions/expression.hpp
        expressions/integer_literal.hpp
        expressions/string_literal.hpp
        expressions/bool_literal.hpp
        expressions/array_literal.hpp
        expressions/unary_operator.hpp
        expressions/binary_operator.hpp
        expressions/name.hpp
        expressions/range.hpp
        expressions/subscript.hpp
        types.hpp
        expressions/typeof.hpp
        values/function.hpp
        expressions/call.hpp
        values/nothing.hpp
        values/function.cpp
        values/value.cpp
        expressions/member_access.hpp
        values/string.cpp
)

if (EMSCRIPTEN)
    add_definitions(-DEMSCRIPTEN)
    target_compile_options(laszlo PRIVATE "-fexceptions")
    set_target_properties(laszlo PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_SOURCE_DIR}/asset_dir/test.las@programs/ --shell-file ${CMAKE_SOURCE_DIR}/emscripten/shell.html -sEXPORTED_FUNCTIONS=_main -fexceptions")

    add_custom_target(RefreshShellFile ALL DEPENDS "${CMAKE_SOURCE_DIR}/emscripten/shell.html")
    add_dependencies(laszlo RefreshShellFile)
endif ()
