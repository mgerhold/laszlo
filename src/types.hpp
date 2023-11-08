#pragma once

#include <format>
#include <memory>
#include <string>

namespace types {
    class BasicType;

    using Type = std::shared_ptr<BasicType>;

    class BasicType {
    public:
        virtual ~BasicType() = default;

        [[nodiscard]] virtual std::string to_string() const = 0;

        [[nodiscard]] virtual bool equals(BasicType const& other) const = 0;
    };

    class I32 final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "I32";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<I32 const*>(&other) != nullptr;
        }
    };

    class Bool final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Bool";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Bool const*>(&other) != nullptr;
        }
    };

    class String final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "String";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<String const*>(&other) != nullptr;
        }
    };

    class Array final : public BasicType {
    private:
        Type m_contained_type;

    public:
        explicit Array(Type contained_type) : m_contained_type{ std::move(contained_type) } { }

        [[nodiscard]] std::string to_string() const override {
            return std::format("[{}]", m_contained_type->to_string());
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            if (auto const other_array = dynamic_cast<Array const*>(&other); other_array != nullptr) {
                return m_contained_type->equals(*other_array->m_contained_type);
            }
            return false;
        }
    };

    class Sentinel final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Sentinel";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Sentinel const*>(&other) != nullptr;
        }
    };

    class RangeIterator final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "RangeIterator";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<RangeIterator const*>(&other) != nullptr;
        }
    };

    class Range final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Range";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Range const*>(&other) != nullptr;
        }
    };

    class ArrayIterator final : public BasicType {
    private:
        Type m_array_type;

    public:
        explicit ArrayIterator(Type array_type) : m_array_type{ std::move(array_type) } { }

        [[nodiscard]] std::string to_string() const override {
            return std::format("ArrayIterator({})", m_array_type->to_string());
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            if (auto const other_iterator = dynamic_cast<ArrayIterator const*>(&other); other_iterator != nullptr) {
                return m_array_type->equals(*other_iterator->m_array_type);
            }
            return false;
        }
    };

    class Unspecified final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "?";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Unspecified const*>(&other) != nullptr;
        }
    };

    class Nothing final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Nothing";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return dynamic_cast<Nothing const*>(&other) != nullptr;
        }
    };

    class Function final : public BasicType {
    public:
        [[nodiscard]] std::string to_string() const override {
            return "Function";
        }

        [[nodiscard]] bool equals(BasicType const& other) const override {
            return false;
        }
    };

    [[nodiscard]] inline bool operator==(Type const& lhs, Type const& rhs) {
        return lhs->equals(*rhs);
    }

    [[nodiscard]] inline Type make_bool() {
        return std::make_shared<Bool>();
    }

    [[nodiscard]] inline Type make_i32() {
        return std::make_shared<I32>();
    }

    [[nodiscard]] inline Type make_string() {
        return std::make_shared<String>();
    }

    [[nodiscard]] inline Type make_sentinel() {
        return std::make_shared<Sentinel>();
    }

    [[nodiscard]] inline Type make_array(Type contained_type) {
        return std::make_shared<Array>(std::move(contained_type));
    }

    [[nodiscard]] inline Type make_array_iterator(Type array_type) {
        return std::make_shared<ArrayIterator>(std::move(array_type));
    }

    [[nodiscard]] inline Type make_range() {
        return std::make_shared<Range>();
    }

    [[nodiscard]] inline Type make_range_iterator() {
        return std::make_shared<RangeIterator>();
    }

    [[nodiscard]] inline Type make_function() {
        return std::make_shared<Function>();
    }

    [[nodiscard]] inline Type make_nothing() {
        return std::make_shared<Nothing>();
    }
} // namespace types
