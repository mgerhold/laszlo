#include "interpreter.hpp"
#include "values/builtin_function.hpp"

void interpret(statements::Statements const& program) {
    auto scope_stack = ScopeStack{};
    scope_stack.top().insert(
            { "split", values::BuiltinFunction::make(BuiltinFunctionType::Split, values::ValueCategory::Rvalue) }
    );
    for (auto const& statement : program) {
        statement->execute(scope_stack);
    }
}
