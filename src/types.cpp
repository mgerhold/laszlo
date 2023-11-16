#include "types.hpp"
#include "statements/struct_definition.hpp"

[[nodiscard]] std::string types::StructType::to_string() const {
    return m_definition->to_string();
}

[[nodiscard]] bool types::StructType::equals(BasicType const& other) const {
    if (auto const other_struct = dynamic_cast<StructType const*>(&other)) {
        return m_definition == other_struct->m_definition;
    }
    return false;
}
