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
            world->System.make<ryn::system::Hierarchy>();

            RYN_INFO("--- STARTING HIERARCHY INVARIANCE TEST ---");

            // 1. Setup
            auto e1 = world->Entity.create();
            world->Component.make<ryn::component::Hierarchy>(e1);

            bool hasRoot = world->Component.exists<ryn::component::HierarchyRoot>(e1);
            RYN_INFO("Test 1 (Creation): Entity {0} has Root Tag: {1}", e1, hasRoot);

            // 2. The "Illegal" Manual Destruction (Testing Self-Healing)
            RYN_INFO("Test 2: Manually destroying Root Tag illegally...");
            world->Component.destroy<ryn::component::HierarchyRoot>(e1);

            hasRoot = world->Component.exists<ryn::component::HierarchyRoot>(e1);
            RYN_INFO("Test 2 (Result): Entity {0} still has Root Tag (Healed): {1}", e1, hasRoot);

            // 3. Parenting (Testing Tag Removal)
            auto e2 = world->Entity.create();
            world->Component.make<ryn::component::Hierarchy>(e2);

            RYN_INFO("Test 3: Setting E1 as child of E2...");
            ryn::system::Hierarchy::setParent(*world, e1, e2);

            hasRoot = world->Component.exists<ryn::component::HierarchyRoot>(e1);
            bool e2HasRoot = world->Component.exists<ryn::component::HierarchyRoot>(e2);
            RYN_INFO("Test 3 (Result): E1 is Root: {0}, E2 is Root: {1}", hasRoot, e2HasRoot);

            // 4. Detaching (Testing Tag Restoration)
            RYN_INFO("Test 4: Detaching E1...");
            ryn::system::Hierarchy::setParent(*world, e1, ryn::service::Entity::Null);

            hasRoot = world->Component.exists<ryn::component::HierarchyRoot>(e1);
            RYN_INFO("Test 4 (Result): E1 is Root again: {0}", hasRoot);

            // 5. Entity Destruction (The Crash Test)
            RYN_INFO("Test 5: Destroying E1 entity-wise...");
            world->Entity.destroy(e1);

            RYN_INFO("--- HIERARCHY TEST COMPLETE ---");
        }

        void onShutdown() override {}
    };
}

ryn::Box<ryn::IApplication> createApplication() { return ryn::Box<rynd::EditorApplication>::make(); }
