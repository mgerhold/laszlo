#pragma once

#include "value.hpp"
#include <string>
#include <unordered_map>
#include <vector>

using Scope = std::unordered_map<std::string, Value>;

class ScopeStack {
private:
    std::vector<std::unique_ptr<Scope>> m_scopes;

public:
    ScopeStack() {
        m_scopes.push_back(std::make_unique<Scope>());
    }

    [[nodiscard]] Scope& top() {
        return *m_scopes.back();
    }

    [[nodiscard]] Scope const& top() const {
        return *m_scopes.back();
    }

    void push(Scope scope) {
        m_scopes.push_back(std::make_unique<Scope>(std::move(scope)));
    }

    [[nodiscard]] std::unique_ptr<Scope> pop() {
        auto result = std::move(m_scopes.back());
        m_scopes.pop_back();
        return std::move(result);
    }

    [[nodiscard]] Value* lookup(std::string const& name) const {
        // todo: refactor return value to tl::optional<Value&>
        for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
            auto const find_iterator = (*it)->find(name);
            if (find_iterator != (*it)->end()) {
                return &find_iterator->second;
            }
        }
        return nullptr;
    }

    [[nodiscard]] std::size_t const size() const {
        return m_scopes.size();
    }

    [[nodiscard]] void truncate(std::size_t const length) {
        assert(length <= m_scopes.size());
        m_scopes.resize(length);
    }
};
