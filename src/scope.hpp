#pragma once

#include "values/value.hpp"
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using Scope = std::unordered_map<std::string, values::Value>;

class ScopeStack {
private:
    /* It's not possible to use an std::vector<Scope> here since MSVC's standard library
     * implementation of std::unordered_map doesn't seem to have its constructors marked
     * noexcept.
     * todo: review if this is now possible that we use std::shared_ptr instead of std::unique_ptr
     */
    std::list<Scope> m_scopes;

public:
    ScopeStack() {
        m_scopes.emplace_back();
    }

    [[nodiscard]] Scope& top() {
        return m_scopes.back();
    }

    [[nodiscard]] Scope const& top() const {
        return m_scopes.back();
    }

    void push(Scope scope) {
        m_scopes.push_back(std::move(scope));
    }

    [[nodiscard]] Scope pop() {
        auto result = std::move(m_scopes.back());
        m_scopes.pop_back();
        return result;
    }

    [[nodiscard]] values::Value* lookup(std::string const& name) {
        // todo: refactor return value to tl::optional<Value&>
        for (auto scope_iterator = m_scopes.rbegin(); scope_iterator != m_scopes.rend(); ++scope_iterator) {
            auto find_iterator = scope_iterator->find(name);
            if (find_iterator != scope_iterator->end()) {
                return &find_iterator->second;
            }
        }
        return nullptr;
    }

    [[nodiscard]] std::size_t size() const {
        return m_scopes.size();
    }

    void truncate(std::size_t const length) {
        assert(length <= m_scopes.size());
        m_scopes.resize(length);
    }
};
