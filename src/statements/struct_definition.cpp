#include "struct_definition.hpp"
#include "../values/struct_type.hpp"

void statements::StructDefinition::execute(ScopeStack& scope_stack) const {
    auto&& [iterator, inserted] =
            scope_stack.top().insert({ std::string{ m_name.lexeme() },
                                       values::StructType::make(this, values::ValueCategory::Rvalue) });
    if (not inserted) {
        throw SymbolRedefinition{ m_name };
    }
}
