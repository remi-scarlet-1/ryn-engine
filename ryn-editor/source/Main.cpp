#include "Entrypoint.h"
#include "Prelude.h"
import Ryntime;
import Ryndition;

RYN_SCOPE_FILE("Main");

namespace rynd {
    class EditorApplication final : public ryn::IApplication {
    protected:
        void onInit() override {
            constexpr ryn::platform::WindowConfig windowConfig{
                .Title        = "RynEditor",
                .Resolution   = {2560, 1440},
                .IsFullscreen = false,
                .IsResizable  = true,
                .IsVisible    = true,
            };

            // clang-format off
            constexpr ryn::platform::GraphicsConfig graphicsConfig{
                .Backend = ryn::platform::GraphicsBackend::Vulkan,
                .VSync   = ryn::platform::GraphicsVSync::Enable
            };
            // clang-format on

            getServiceStack().push<ryn::platform::Window>(windowConfig);
            getServiceStack().push<ryn::platform::Input>();
            getServiceStack().push<ryn::platform::Graphics>(graphicsConfig);
            getServiceStack().push<Editor>();
        }

        void onShutdown() override {}
    };
}

ryn::Box<ryn::IApplication> createApplication() { return ryn::Box<rynd::EditorApplication>::make(); }
