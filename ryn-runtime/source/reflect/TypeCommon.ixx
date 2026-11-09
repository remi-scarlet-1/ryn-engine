module;
#include "Prelude.h"
#include <cstdint>
#include <format>
#include <functional>

export module Ryntime.Reflect:TypeCommon;
import Ryntime.Logger;
export import Ryntime.Container;

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

    struct TypeID : Identity<TypeID> {};

    struct TypeCategory : Tag<TypeCategory> {
    public:
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
        const char*  Name      = "<unknown>";
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
    inline auto    format(const ryn::TypeID& id, std::format_context& ctx) const { return std::format_to(ctx.out(), "0x{:X}", id.Hash); }
};

template<>
struct std::formatter<ryn::TypeCategory> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }
    inline auto    format(const ryn::TypeCategory cat, std::format_context& ctx) const { return std::format_to(ctx.out(), "{}", cat.Name); }
};

template<>
struct std::formatter<ryn::TypeInfo> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }
    inline auto    format(const ryn::TypeInfo& info, std::format_context& ctx) const {
        const char* name     = info.Name ? info.Name : "<undefined>";
        const char* category = info.Category.Name;
        return std::format_to(ctx.out(), "[ID: {}, Category: {}, Name: {}]", info.ID, category, name);
    }
};