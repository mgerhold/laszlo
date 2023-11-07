#pragma once

#include "../scope.hpp"
#include "../token.hpp"
#include "../values/value.hpp"
#include <variant>

namespace expressions {
    class Expression {
    protected:
        Expression() = default;

    public:
        virtual ~Expression() = default;

        [[nodiscard]] virtual values::Value evaluate(ScopeStack& scope_stack) const = 0;

        [[nodiscard]] virtual SourceLocation source_location() const = 0;
    };
} // namespace expressions
