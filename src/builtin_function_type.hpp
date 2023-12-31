#pragma once

#include <cassert>
#include <string_view>

enum class BuiltinFunctionType {
    Split,
    Join,
    Delete,
    Write,
    Read,
    Trim,
};

[[nodiscard]] constexpr std::string_view to_view(BuiltinFunctionType const type) {
    switch (type) {
        case BuiltinFunctionType::Split:
            return "split";
        case BuiltinFunctionType::Join:
            return "join";
        case BuiltinFunctionType::Delete:
            return "delete";
        case BuiltinFunctionType::Write:
            return "write";
        case BuiltinFunctionType::Read:
            return "read";
        case BuiltinFunctionType::Trim:
            return "trim";
    }
    assert(false and "unreachable");
    return "";
}
