#include "function_declaration.hpp"
#include "../values/function.hpp"

namespace statements {
    void FunctionDeclaration::execute(ScopeStack& scope_stack) const {
        auto name = std::string{ m_name.lexeme() };
        if (scope_stack.top().contains(name)) {
            throw VariableRedefinition{ m_name };
        }
        scope_stack.top().insert({ std::move(name),
                                   values::Function::make(
                                           m_name,
                                           m_parameters,
                                           m_return_type,
                                           m_body.get(),
                                           values::ValueCategory::Lvalue
                                   ) });
    }
} // namespace statements
