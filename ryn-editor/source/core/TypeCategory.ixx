module;
#include "Prelude.h"

export module Ryndition.Core:TypeCategory;
export import Ryntime;

using namespace ryn::reflection;

export namespace rynd::core {
    struct EditorTypeCategory {
        static constexpr TypeCategory Panel = TypeCategory::generate("Editor.Panel");
    };
}