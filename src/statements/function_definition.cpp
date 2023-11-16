#include "../values/function.hpp"
#include "function_definition.hpp"

namespace statements {
    void FunctionDefinition::execute(ScopeStack& scope_stack) const {
        auto name = std::string{ m_name.lexeme() };
        if (scope_stack.top().contains(name)) {
            throw SymbolRedefinition{ m_name };
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
