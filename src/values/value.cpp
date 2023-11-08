#include "value.hpp"
#include "../expressions/expression.hpp"

namespace values {
    // clang-format off
    [[nodiscard]] Value BasicValue::call(
            ScopeStack& scope_stack, [[maybe_unused]] std::vector<std::unique_ptr<expressions::Expression>> const&
    ) const {
        // clang-format on
        throw OperationNotSupportedByType{ "call", type() };
    }
} // namespace values
