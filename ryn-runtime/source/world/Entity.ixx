module;
#include "Prelude.h"
#include <cstdint>
#include <entt/entt.hpp>

export module Ryntime.World:Entity;
export import Ryntime.Core;

export namespace ryn::world {
    struct Entity {
        static constexpr Entity Null() { return Entity(static_cast<entt::entity>(entt::null)); }

        constexpr Entity(entt::entity e) noexcept : id(static_cast<ENTT_ID_TYPE>(e)) {}

        constexpr operator ENTT_ID_TYPE() const noexcept { return id; }
        constexpr operator entt::entity() const noexcept { return static_cast<entt::entity>(id); }

        [[nodiscard]] constexpr bool isValid() const noexcept { return id != static_cast<ENTT_ID_TYPE>(entt::null); }
        explicit constexpr           operator bool() const noexcept { return isValid(); }
        auto                         operator<=>(const Entity&) const = default;

    private:
        ENTT_ID_TYPE id = entt::null;
    };
}

template<>
struct std::formatter<ryn::world::Entity> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::world::Entity entity, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<ENTT_ID_TYPE>(entity));
    }
};