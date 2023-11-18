#include "interpreter.hpp"
#include "values/builtin_function.hpp"

void interpret(statements::Statements const& program) {
    auto scope_stack = ScopeStack{};
    scope_stack.top().insert(
            { "split", values::BuiltinFunction::make(BuiltinFunctionType::Split, values::ValueCategory::Rvalue) }
    );
    scope_stack.top().insert(
            { "join", values::BuiltinFunction::make(BuiltinFunctionType::Join, values::ValueCategory::Rvalue) }
    );
    scope_stack.top().insert(
            { "delete", values::BuiltinFunction::make(BuiltinFunctionType::Delete, values::ValueCategory::Rvalue) }
    );
    scope_stack.top().insert(
            { "write", values::BuiltinFunction::make(BuiltinFunctionType::Write, values::ValueCategory::Rvalue) }
    );
    scope_stack.top().insert(
            { "read", values::BuiltinFunction::make(BuiltinFunctionType::Read, values::ValueCategory::Rvalue) }
    );
    for (auto const& statement : program) {
        statement->execute(scope_stack);
    }
}
