module;
#include "Prelude.h"
#include <concepts>

export module Ryntime.World:Component;
export import Ryntime.Core;

using namespace ryn::reflection;

namespace ryn::world {
    struct ComponentTag {};
}

export namespace ryn::world {
    template<TypeName Name>
    struct IComponent : ComponentTag, StaticType<TypeCategory::Component, Name> {};

    template<typename T>
    concept IsComponent = std::derived_from<T, ComponentTag> && IsType<T>;
}