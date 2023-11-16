#include "struct_literal.hpp"
#include "../values/struct_type.hpp"
#include "../values/value.hpp"

values::Value expressions::StructLiteral::evaluate(ScopeStack& scope_stack) const {
    auto const type = scope_stack.lookup(std::string{ m_name.lexeme() });
    if (type == nullptr) {
        throw UndefinedReference{ m_name };
    }
    if (not(*type)->is_struct_type()) {
        throw UnknownType{ m_name };
    }

    auto const& definition = (*type)->as_struct_type();

    auto value_members = std::unordered_map<std::string, values::Value>{};

    for (auto const& [name, value] : m_initializers) {
        auto const& [iterator, inserted] =
                value_members.insert({ std::string{ name.lexeme() }, value->evaluate(scope_stack) });
        if (not inserted) {
            // todo: throw different exception type
            throw std::runtime_error{ "duplicate struct initializer" };
        }
    }

    for (auto const& [name, type] : definition.value()->members()) {
        auto const find_iterator = value_members.find(std::string{ name.lexeme() });
        auto const found = (find_iterator != value_members.end());
        if (not found) {
            // todo: throw different exception type
            throw std::runtime_error{ "missing initializer for struct member" };
        }
        if (find_iterator->second->type() != type) {
            // todo: throw different exception type
            throw std::runtime_error{ "struct member initializer has wrong type" };
        }
    }

    if (value_members.size() > definition.value()->members().size()) {
        // todo: throw different exception type
        throw std::runtime_error{ "too many struct member initializers" };
    }

    return values::Struct::make(
            definition.as_struct_type().value(),
            std::move(value_members),
            values::ValueCategory::Rvalue
    );
}
