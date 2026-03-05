#include "Entrypoint.h"
#include "Prelude.h"
import Ryntime;
import Ryndition.Service;

RYN_SCOPE_FILE("Main");

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

        constexpr ryn::service::GraphicsConfig graphicsConfig{
            // clang-format off
            .Backend = ryn::service::GraphicsBackend::Vulkan,
            .VSync   = ryn::service::GraphicsVSync::Enable
            // clang-format on
        };

        // Setup services
        getServiceStack().push<ryn::service::Window>(windowConfig);
        getServiceStack().push<ryn::service::Input>();
        getServiceStack().push<ryn::service::Graphics>(graphicsConfig);
        getServiceStack().push<ryn::service::World>();
        getServiceStack().push<rynd::service::Editor>();
    }

    void onShutdown() override {}
};
ryn::Box<ryn::IApplication> createApplication() { return ryn::Box<EditorApplication>::make(); }
