module;
#include "Prelude.h"

export module Ryndition.Core:TypeCategory;
export import Ryntime;

export namespace rynd::core {
    struct EditorTypeCategory {
        static constexpr ryn::TypeCategory Panel = ryn::TypeCategory::generate("Editor.Panel");
    };
}