module;
#include "Prelude.h"

export module Ryndition.Panel:Hierarchy;
export import Ryndition;

export namespace rynd::panel {
    class Hierarchy : public IEditorPanel {
    public:
        Hierarchy() = default;
        ~Hierarchy() override = default;

        void onUpdate(const ryn::FrameTime& time) override {

        }

        void onDraw(const ryn::FrameTime& time) override {
            
        }

        const char* getName() const override { return "Hierarchy"; }
    };
}