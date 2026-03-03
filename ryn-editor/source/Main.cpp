#include "Entrypoint.h"
#include "Prelude.h"
import Ryntime;
import Ryndition;

RYN_SCOPE_FILE("Main");

namespace rynd {
    class EditorApplication final : public ryn::IApplication {
    protected:
        void onInit() override {
            constexpr ryn::service::WindowConfig windowConfig{
                .Title        = "RynEditor",
                .Resolution   = {2560, 1440},
                .IsFullscreen = false,
                .IsResizable  = true,
                .IsVisible    = true,
            };

            // clang-format off
            constexpr ryn::service::GraphicsConfig graphicsConfig{
                .Backend = ryn::service::GraphicsBackend::Vulkan,
                .VSync   = ryn::service::GraphicsVSync::Enable
            };
            // clang-format on

            getServiceStack().push<ryn::service::Window>(windowConfig);
            getServiceStack().push<ryn::service::Input>();
            getServiceStack().push<ryn::service::Graphics>(graphicsConfig);
            getServiceStack().push<ryn::service::World>();
            getServiceStack().push<Editor>();
        }

        void onShutdown() override {}
    };
}

ryn::Box<ryn::IApplication> createApplication() { return ryn::Box<rynd::EditorApplication>::make(); }
