#include "value.hpp"
#include "../expressions/expression.hpp"
#include "string.hpp"

namespace values {
    // clang-format off
    [[nodiscard]] Value BasicValue::call(
            ScopeStack& scope_stack, [[maybe_unused]] std::vector<std::unique_ptr<expressions::Expression>> const&
    ) const {
        // clang-format on
        throw OperationNotSupportedByType{ "call", type() };
    }

    [[nodiscard]] Value BasicValue::cast(types::Type const& target_type) const {
        if (target_type == types::make_string()) {
            return String::make(string_representation(), ValueCategory::Rvalue);
        }
        throw OperationNotSupportedByType{ std::format("cast to type {}", target_type->to_string()), type() };
    }
} // namespace values
