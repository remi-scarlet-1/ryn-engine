module;
#include "Prelude.h"
#include <cstdint>
#include <format>
#include <functional>

export module Ryntime.Core.Reflection:TypeCommon;

export namespace ryn {
    template<size_t N>
    struct TypeName {
        char value[N]{};
        consteval TypeName(const char (&str)[N]) {
            for (size_t i = 0; i < N; ++i)
                value[i] = str[i];
        }
        [[nodiscard]] constexpr const char* data() const { return value; }
    };

    struct TypeID {
    public:
        static constexpr uint64_t INVALID_HASH = 0xDEADBEEFDEADBEEFull;
        uint64_t                  Hash         = INVALID_HASH;

        constexpr TypeID() = default;
        constexpr explicit TypeID(const uint64_t hash) : Hash(hash) {}

        static constexpr TypeID invalid() { return TypeID{}; }
        static consteval TypeID generate(const char* str) {
            uint64_t hash = 0xcbf29ce484222325;
            while (*str) {
                hash ^= static_cast<uint64_t>(*str++);
                hash *= 0x100000001b3;
            }

            return TypeID(hash);
        }

        constexpr bool     operator==(const TypeID& other) const { return Hash == other.Hash; }
        constexpr auto     operator<=>(const TypeID& other) const { return Hash <=> other.Hash; }
        explicit constexpr operator bool() const { return Hash != INVALID_HASH; }
    };

    struct TypeCategory {
    public:
        static constexpr uint32_t INVALID_HASH = 0xDEADBEEF;
        uint32_t                  Hash         = INVALID_HASH;
        char                      Name[60]     = "<invalid>";

        consteval TypeCategory() = default;

        static constexpr TypeCategory invalid() { return TypeCategory{}; }
        static consteval TypeCategory generate(const char* str) {
            TypeCategory cat{};
            cat.Hash = 0x811c9dc5;

            int i = 0;
            while (str[i]) {
                cat.Hash ^= static_cast<uint32_t>(str[i]);
                cat.Hash *= 0x01000193;

                // Copy string into the fixed buffer, leave room for null terminator
                if (i < 59) cat.Name[i] = str[i];
                i++;
            }
            cat.Name[i < 59 ? i : 59] = '\0';

            return cat;
        }

        constexpr bool     operator==(const TypeCategory& other) const { return Hash == other.Hash; }
        constexpr auto     operator<=>(const TypeCategory& other) const { return Hash <=> other.Hash; }
        explicit constexpr operator bool() const { return Hash != INVALID_HASH; }

        /* Well known engine type categories */
        static const TypeCategory None;
        static const TypeCategory Service;
        static const TypeCategory Event;
        static const TypeCategory System;
        static const TypeCategory Component;
        static const TypeCategory ComponentRequest;
    };

    constexpr TypeCategory TypeCategory::None{generate("Ryn.None")};
    constexpr TypeCategory TypeCategory::Service{generate("Ryn.Service")};
    constexpr TypeCategory TypeCategory::Event{generate("Ryn.Event")};
    constexpr TypeCategory TypeCategory::System{generate("Ryn.Game.System")};
    constexpr TypeCategory TypeCategory::Component{generate("Ryn.Game.Component")};
    constexpr TypeCategory TypeCategory::ComponentRequest{generate("Ryn.Game.ComponentRequest")};

    struct TypeInfo {
        TypeID       ID        = TypeID::invalid();
        TypeCategory Category  = TypeCategory::None;
        const char*  Name      = "<anonymous>";
        bool         IsDynamic = false;
    };
}

template<>
struct std::hash<ryn::TypeID> {
    inline size_t operator()(const ryn::TypeID& id) const noexcept { return id.Hash; }
};

template<>
struct std::hash<ryn::TypeCategory> {
    inline size_t operator()(const ryn::TypeCategory& category) const noexcept { return category.Hash; }
};

template<>
struct std::formatter<ryn::TypeID> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::TypeID& id, std::format_context& ctx) const { return std::format_to(ctx.out(), "0x{:X}", id.Hash); }
};

template<>
struct std::formatter<ryn::TypeCategory> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::TypeCategory cat, std::format_context& ctx) const { return std::format_to(ctx.out(), "{}", cat.Name); }
};

template<>
struct std::formatter<ryn::TypeInfo> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::TypeInfo& info, std::format_context& ctx) const {
        const char* name     = info.Name ? info.Name : "<undefined>";
        const char* category = info.Category.Name;
        return std::format_to(ctx.out(), "[ID: {}, Category: {}, Name: {}]", info.ID, category, name);
    }
};