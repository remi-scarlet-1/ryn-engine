module;
#include "Prelude.h"

export module Ryndition.Core:EditorTypeCategory;
export import Ryntime;

using ryn::TypeCategory;

export namespace rynd {
    struct EditorTypeCategory {
        static constexpr TypeCategory Panel = TypeCategory::generate("Editor.Panel");
    };
}