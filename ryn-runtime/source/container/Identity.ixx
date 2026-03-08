module;
#include "Prelude.h"
#include <cstdint>

export module Ryntime.Container:Identity;

export namespace ryn {
    /**
     * @struct Identity
     * @brief A pure 64-bit hashed identifier derived from a string.
     * Use this for hot-paths, ECS keys, and type-erasure.
     */
    template<typename Discriminator>
    struct Identity {
        static constexpr uint64_t INVALID_HASH = 0xDEADBEEFDEADBEEFull;
        uint64_t                  Hash         = INVALID_HASH;

        consteval Identity() = default;
        consteval explicit Identity(uint64_t hash) : Hash(hash) {}

        static consteval Identity invalid() { return Identity{}; }
        static consteval Identity generate(const char* str) {
            uint64_t hash = 0xcbf29ce484222325;
            while (*str) {
                hash ^= static_cast<uint64_t>(*str++);
                hash *= 0x100000001b3;
            }
            return Identity{hash};
        }

        consteval operator Discriminator() const { return Discriminator{*this}; }

        constexpr bool     operator==(const Identity& other) const { return Hash == other.Hash; }
        constexpr auto     operator<=>(const Identity& other) const { return Hash <=> other.Hash; }
        explicit constexpr operator bool() const { return Hash != INVALID_HASH; }
    };

    /**
     * @struct Tag
     * @brief A 64-byte descriptive identifier that carries its string name.
     * Use this for Categories, Memory Tags, and Editor metadata.
     */
    template<typename Discriminator, uint32_t BufferSize = 56>
    struct Tag : Identity<Discriminator> {
        char Name[BufferSize] = "<invalid>";

        consteval Tag() = default;
        consteval Tag(Identity<Discriminator> id, const char* name) : Identity<Discriminator>(id) {
            uint32_t i = 0;
            while (name[i] && i < (BufferSize - 1)) {
                Name[i] = name[i];
                i++;
            }
            while (i < BufferSize)
                Name[i++] = '\0';
        }

        static consteval Tag generate(const char* str) { return Tag{Identity<Discriminator>::generate(str), str}; }

        consteval operator Discriminator() const { return Discriminator{*this}; }
    };
}