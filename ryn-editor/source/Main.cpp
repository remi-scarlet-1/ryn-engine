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
            auto world = getServiceStack().push<ryn::service::World>();
            getServiceStack().push<Editor>();

            // TODO: remove hierarchy system test
            world->systemMake<ryn::game::HierarchySystem>();

            auto entity = world->entityCreate();
            auto entity2 = world->entityCreate();
            world->componentMake<ryn::game::HierarchyComponent>(entity);
            world->componentMake<ryn::game::HierarchyComponent>(entity2);

            ryn::game::HierarchySystem::setParent(*world, entity, entity2);
        }

        void onShutdown() override {}
    };
}

ryn::Box<ryn::IApplication> createApplication() { return ryn::Box<rynd::EditorApplication>::make(); }
