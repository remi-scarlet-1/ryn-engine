module;
#include "Prelude.h"

export module Ryndition.Panel:Hierarchy;
export import Ryntime;
export import :IPanel;

export namespace rynd::panel {
    class Hierarchy : public IPanel {
    public:
        Hierarchy()           = default;
        ~Hierarchy() override = default;

        const char* getName() const override { return "Hierarchy"; }

        void onConfigure(ryn::ServiceDiscovery& discovery) override {}
        void onDraw(const ryn::FrameTime& time) override {}
    };
}