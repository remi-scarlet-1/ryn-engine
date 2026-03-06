module;
#include "Prelude.h"
#include <cstdint>
#include <format>
#include <functional>

export module Ryntime.Core.Reflection:TypeCommon;

export namespace ryn::reflection {
    struct TypeID {
        static constexpr TypeID invalid() { return TypeID{}; }
        static consteval TypeID generateHash(const char* str) {
            uint64_t hash = 0xcbf29ce484222325;
            while (*str) {
                hash ^= static_cast<uint64_t>(*str++);
                hash *= 0x100000001b3;
            }

            return TypeID(hash);
        }

        [[nodiscard]] constexpr uint64_t value() const { return Hash; }

        constexpr bool     operator==(const TypeID& other) const noexcept { return Hash == other.Hash; }
        constexpr bool     operator!=(const TypeID& other) const noexcept { return Hash != other.Hash; }
        constexpr bool     operator<(const TypeID& other) const noexcept { return Hash < other.Hash; }
        constexpr bool     operator<=(const TypeID& other) const noexcept { return Hash <= other.Hash; }
        constexpr bool     operator>(const TypeID& other) const noexcept { return Hash > other.Hash; }
        constexpr bool     operator>=(const TypeID& other) const noexcept { return Hash >= other.Hash; }
        explicit constexpr operator bool() const { return Hash != INVALID_HASH; }

    private:
        static constexpr uint64_t INVALID_HASH = 0xDEADBEEFDEADBEEFull;
        uint64_t                  Hash         = INVALID_HASH;

        constexpr TypeID() = default;
        constexpr explicit TypeID(const uint64_t hash) : Hash(hash) {}
    };

    enum class TypeCategory : uint16_t {
        None             = 0,
        Service          = 1,
        Event            = 2,
        System           = 3,
        Component        = 4,
        ComponentRequest = 5,
        Count
    };

    [[nodiscard]] constexpr const char* typeCategoryToString(TypeCategory category) noexcept {
        switch (category) {
            case TypeCategory::None: return "Other";
            case TypeCategory::Service: return "Service";
            case TypeCategory::Event: return "Event";
            case TypeCategory::System: return "System";
            case TypeCategory::Component: return "Component";
            case TypeCategory::ComponentRequest: return "ComponentRequest";
            case TypeCategory::Count: return "<internal_counter>";
        }
        return "<undefined>";
    }

    struct TypeInfo {
        const char*  Name      = "<invalid>";
        TypeID       ID        = TypeID::invalid();
        TypeCategory Category  = TypeCategory::None;
        bool         IsDynamic = false;
    };
}

template<>
struct std::hash<ryn::reflection::TypeID> {
    inline size_t operator()(const ryn::reflection::TypeID& id) const noexcept { return id.value(); }
};

template<>
struct std::formatter<ryn::reflection::TypeID> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::reflection::TypeID& id, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "0x{:X}", id.value());
    }
};

template<>
struct std::formatter<ryn::reflection::TypeCategory> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::reflection::TypeCategory cat, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", ryn::reflection::typeCategoryToString(cat));
    }
};

template<>
struct std::formatter<ryn::reflection::TypeInfo> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::reflection::TypeInfo& info, std::format_context& ctx) const {
        const char* name     = info.Name ? info.Name : "<undefined>";
        const char* category = ryn::reflection::typeCategoryToString(info.Category);
        return std::format_to(ctx.out(), "[ID: {}, Category: {}, Name: {}]", info.ID, category, name);
    }
};