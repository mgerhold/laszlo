#pragma once
#include "../expressions/expression.hpp"
#include "../scope.hpp"
#include <memory>
#include <vector>

namespace statements {

    class Statement {
    protected:
        Statement() = default;

    public:
        virtual ~Statement() = default;

        virtual void execute(ScopeStack& scope_stack) const = 0;
    };

    using Statements = std::vector<std::unique_ptr<Statement>>;

} // namespace statements
