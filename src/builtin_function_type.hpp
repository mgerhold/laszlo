#pragma once

#include <cassert>
#include <string_view>

enum class BuiltinFunctionType {
    Split,
};

[[nodiscard]] constexpr std::string_view to_view(BuiltinFunctionType const type) {
    switch (type) {
        case BuiltinFunctionType::Split:
            return "split";
    }
    assert(false and "unreachable");
    return "";
}
