#pragma once

#include "ostream_formatter.hpp"
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <utility>

struct Token;

struct SourceLocation {
    std::string_view filename;
    std::string_view source;
    std::size_t byte_offset;
    std::size_t num_bytes;

    [[nodiscard]] static SourceLocation from_range(SourceLocation const& first, SourceLocation const& last) {
        assert(first.filename == last.filename);
        assert(first.byte_offset < last.byte_offset + last.num_bytes);
        auto start_offset = first.byte_offset;
        auto end_offset = last.byte_offset + last.num_bytes;
        auto num_bytes = end_offset - start_offset;
        return SourceLocation{ first.filename, first.source, start_offset, num_bytes };
    }

    [[nodiscard]] std::pair<std::size_t, std::size_t> line_and_column() const {
        assert(byte_offset < source.length());
        auto line = std::size_t{ 1 };
        auto column = std::size_t{ 1 };
        for (std::size_t i = 0; i < byte_offset; ++i) {
            if (source.at(i) == '\n') {
                ++line;
                column = 1;
            } else {
                ++column;
            }
        }
        return { line, column };
    }

    [[nodiscard]] std::string_view text() const {
        return source.substr(byte_offset, num_bytes);
    }

    friend std::ostream& operator<<(std::ostream& os, SourceLocation const& source_location) {
        auto const [line, column] = source_location.line_and_column();
        return os << source_location.filename << ':' << line << ':' << column;
    }
};

template<>
struct std::formatter<SourceLocation> : ostream_formatter { };
