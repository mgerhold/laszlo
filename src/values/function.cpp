#include "function.hpp"
#include "../statement.hpp"

namespace values {
    [[nodiscard]] Value Function::call(ScopeStack& scope_stack) const {
        m_body->execute(scope_stack);
        return values::Nothing::make(ValueCategory::Rvalue);
    }
} // namespace values
