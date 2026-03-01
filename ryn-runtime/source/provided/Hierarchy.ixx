module;
#include "Prelude.h"

#include <entt/entity/entity.hpp>

export module Ryntime.Provided:Hierarchy;
export import Ryntime.World;

using namespace ryn::reflection;
using namespace ryn::world;

RYN_SCOPE_FILE("Hierarchy");

export namespace ryn::provided {
    struct HierarchyRoot : IComponent<"ryn::provided::HierarchyRoot"> {};

    struct Hierarchy : IComponent<"ryn::provided::Hierarchy"> {
        Entity Parent      = Entity::Null();
        Entity FirstChild  = Entity::Null();
        Entity NextSibling = Entity::Null();
        Entity PrevSibling = Entity::Null();
    };

    class RYN_API HierarchyProcessor final : public DynamicType<IProcessor, "ryn::provided::HierarchyProcessor"> {
    public:
        static void setParent(World& world, Entity entity, Entity newParent) {
            RYN_ASSERT(entity && newParent, "Cannot operate on null entity.");
            RYN_ASSERT(entity != newParent, "Cannot parent an entity to itself.");
            RYN_ASSERT(!isAncestorOf(world, entity, newParent), "Circular hierarchy detected!");

            // Ensure entity is detached from any existing parent/sibings before re-linking
            clearParent(world, entity);

            // Remove root tag (it's becoming a child)
            world.Component.destroy<HierarchyRoot>(entity);

            // Get/Make hierarchy components
            auto& eH = world.Component.getOrMake<Hierarchy>(entity);
            auto& pH = world.Component.getOrMake<Hierarchy>(newParent);

            // Link entity to parent
            eH.Parent      = newParent;
            eH.NextSibling = pH.FirstChild;
            eH.PrevSibling = Entity::Null();

            // Link siblings to entity
            if (eH.NextSibling) {
                RYN_ASSERT(world.Component.has<Hierarchy>(eH.NextSibling),
                           "Hierarchy Invariant Violated: Sibling missing Hierarchy component.");
                auto& nextH       = world.Component.get<Hierarchy>(eH.NextSibling);
                nextH.PrevSibling = entity;
            }

            // Link parent to entity
            pH.FirstChild = entity;
        }

        static void clearParent(World& world, Entity entity) {
            // Repair the tree links around this entity
            repairTreeLinks(world, entity);

            // Reset the entity's hierarchy state
            auto& h       = world.Component.getOrMake<Hierarchy>(entity);
            h.Parent      = Entity::Null();
            h.NextSibling = Entity::Null();
            h.PrevSibling = Entity::Null();

            // Register as a root since it no longer has a parent
            world.Component.getOrMake<HierarchyRoot>(entity);
        }

        static bool isAncestorOf(World& world, Entity potentialAncestor, Entity entity) {
            Entity current = entity;
            while (current) {
                if (current == potentialAncestor) return true;
                if (auto* h = world.Component.tryGet<Hierarchy>(current)) current = h->Parent;
                else break;
            }
            return false;
        }

    private:
        ObserverConnection m_OnDestroyed;

        void onConfigure(ServiceDiscovery& discovery) override {
            m_OnDestroyed = m_World->Observer.onComponentDestroy<Hierarchy, &HierarchyProcessor::onCleanup>(this);
        }

        void onCleanup(Entity e) {
            // Fetch the component. It's safe because the observer fires before deletion.
            auto& h = m_World->Component.get<Hierarchy>(e);

            // Iterate through children recursively and destroy them
            Entity child = h.FirstChild;
            while (child) {
                RYN_ASSERT(m_World->Component.has<Hierarchy>(child), "Hierarchy Invariant Violated: Child missing Hierarchy component.");

                // Fetch next sibling before child's Hierarchy component is deleted
                Entity next = m_World->Component.get<Hierarchy>(child).NextSibling;
                m_World->Entity.destroy(child);
                child = next;
            }

            // Fix e's parent and siblings so they don't point to e anymore
            repairTreeLinks(*m_World, e);

            // Cleanup root tag if exists
            m_World->Component.destroy<HierarchyRoot>(e);
        }

        static void repairTreeLinks(World& world, Entity entity) {
            auto* h = world.Component.tryGet<Hierarchy>(entity);
            if (!h) return;

            // Fix Sibling Links
            if (h->PrevSibling) {
                RYN_ASSERT(world.Component.has<Hierarchy>(h->PrevSibling),
                           "Hierarchy Invariant Violated: Prev Sibling missing Hierarchy component.");
                world.Component.get<Hierarchy>(h->PrevSibling).NextSibling = h->NextSibling;
            }

            if (h->NextSibling) {
                RYN_ASSERT(world.Component.has<Hierarchy>(h->NextSibling),
                           "Hierarchy Invariant Violated: Next Sibling missing Hierarchy component.");
                world.Component.get<Hierarchy>(h->NextSibling).PrevSibling = h->PrevSibling;
            }

            // Fix Parent Link
            if (h->Parent) {
                RYN_ASSERT(world.Component.has<Hierarchy>(h->Parent), "Hierarchy Invariant Violated: Parent missing Hierarchy component.");
                auto& parH = world.Component.get<Hierarchy>(h->Parent);
                if (parH.FirstChild == entity) { parH.FirstChild = h->NextSibling; }
            }
        }
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::provided::HierarchyRoot);
RYN_TYPE_REFLECTION_KEEP(ryn::provided::Hierarchy);
RYN_TYPE_REFLECTION_KEEP(ryn::provided::HierarchyProcessor);