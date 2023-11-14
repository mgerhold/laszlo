#pragma once

#include <cassert>
#include <string_view>

enum class BuiltinFunctionType {
    Split,
    Join,
};

[[nodiscard]] constexpr std::string_view to_view(BuiltinFunctionType const type) {
    switch (type) {
        case BuiltinFunctionType::Split:
            return "split";
        case BuiltinFunctionType::Join:
            return "join";
    }
    assert(false and "unreachable");
    return "";
}
