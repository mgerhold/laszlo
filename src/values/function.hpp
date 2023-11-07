#pragma once

#include "../scope.hpp"
#include "nothing.hpp"
#include "value.hpp"

class Statement;

namespace values {

    class Function final : public BasicValue {
    private:
        Statement const* m_body;

    public:
        Function(Statement const* const body, ValueCategory const value_category)
            : BasicValue{ value_category },
              m_body{ body } { }

        [[nodiscard]] static Value make(Statement const* const body, ValueCategory const value_category) {
            return std::make_shared<Function>(body, value_category);
        }

        [[nodiscard]] std::string string_representation() const override {
            return "Function()";
        }

        [[nodiscard]] types::Type type() const override {
            return types::make_function();
        }

        [[nodiscard]] Value clone() const override {
            return make(m_body, value_category());
        }

        [[nodiscard]] Value call(ScopeStack& scope_stack) const override;
    };

} // namespace values
