module;
#include "Prelude.h"

export module Ryntime.Game:Hierarchy;
export import Ryntime.Service;
export import Ryntime.Core.Reflection;

using namespace ryn::service;
using namespace ryn::reflection;

RYN_SCOPE_FILE("Game|HierarchySystem");

export namespace ryn::game {
    class HierarchySystem;

    struct HierarchyRootTag : IComponent<"ryn::game::HierarchyRootTag"> {};

    struct HierarchyComponent : IComponent<"ryn::game::HierarchyComponent"> {
        inline Entity getParent() const { return Parent; }
        inline Entity getFirstChild() const { return First; }
        inline Entity getNextSibling() const { return Next; }
        inline Entity getPrevSibling() const { return Prev; }

    private:
        friend HierarchySystem;
        Entity Parent = Entity::Null;
        Entity First  = Entity::Null;
        Entity Next   = Entity::Null;
        Entity Prev   = Entity::Null;
    };

    class RYN_API HierarchySystem final : public DynamicType<ISystem, "ryn::game::HierarchySystem"> {
    public:
        static void setParent(World& world, Entity entity, Entity parent) {
            if (!entity) return;

            auto currentComp = world.componentGetOrMake<HierarchyComponent>(entity);
            if (currentComp->Parent == parent) return;

            // Cycle detection
            if (parent && isAncestorOf(world, entity, parent)) {
                RYN_WARN("Circular hierarchy detected. Operation aborted.");
                return;
            }

            // Detach from current hierarchy
            detachFromHierarchy(world, entity);
            if (!parent) return; // Already detached, now a root.

            // Attach to new parent
            auto* parentComp = world.componentGetOrMake<HierarchyComponent>(parent);

            Entity oldFirst     = parentComp->First;
            currentComp->Parent = parent;
            currentComp->Next   = oldFirst;
            currentComp->Prev   = Entity::Null;

            if (oldFirst) world.componentGet<HierarchyComponent>(oldFirst)->Prev = entity;

            parentComp->First = entity;

            // Remove root tag, no longer a root
            world.componentDestroy<HierarchyRootTag>(entity);
        }

        static void detachFromHierarchy(World& world, Entity entity) {
            auto* current = world.componentTryGet<HierarchyComponent>(entity);
            if (!current) return;

            world.componentGetOrMake<HierarchyRootTag>(entity);
            if (!current->Parent) return;

            auto* parentComp = world.componentTryGet<HierarchyComponent>(current->Parent);
            if (parentComp) {
                if (parentComp->First == entity) parentComp->First = current->Next;
                if (current->Prev) world.componentGet<HierarchyComponent>(current->Prev)->Next = current->Next;
                if (current->Next) world.componentGet<HierarchyComponent>(current->Next)->Prev = current->Prev;
            }

            current->Parent = Entity::Null;
            current->Next   = Entity::Null;
            current->Prev   = Entity::Null;
        }

        static bool isAncestorOf(World& world, Entity ancestor, Entity descendant) {
            Entity current = descendant;
            while (current) {
                if (current == ancestor) return true;
                auto* comp = world.componentTryGet<HierarchyComponent>(current);
                current    = comp ? comp->Parent : Entity::Null;
            }
            return false;
        }

    private:
        ObserverConnection m_OnTagMake;
        ObserverConnection m_OnHierarchyMake;
        ObserverConnection m_OnHierarchyDestroy;

        void onConfigure(ServiceDiscovery& discovery) override {
            m_OnTagMake          = world().onComponentMake<HierarchyRootTag, &HierarchySystem::onTagMake>(this);
            m_OnHierarchyMake    = world().onComponentMake<HierarchyComponent, &HierarchySystem::onHierarchyMake>(this);
            m_OnHierarchyDestroy = world().onComponentDestroy<HierarchyComponent, &HierarchySystem::onHierarchyDestroy>(this);
        }

        void onTagMake(Entity entity) {
            // If wrong conditions, force root tag back off
            auto* comp = world().componentTryGet<HierarchyComponent>(entity);
            if (!comp || comp->Parent) world().componentDestroy<HierarchyRootTag>(entity);
        }

        void onHierarchyMake(Entity entity) {
            // New hierarchy components are root by default
            world().componentGetOrMake<HierarchyRootTag>(entity);
        }

        void onHierarchyDestroy(Entity entity) {
            detachFromHierarchy(world(), entity);

            auto current = world().componentGet<HierarchyComponent>(entity);

            Entity child = current->First;
            while (child) {
                auto   childHierarchy = world().componentTryGet<HierarchyComponent>(child);
                Entity nextSibling    = childHierarchy ? childHierarchy->Next : Entity::Null;

                // Destroying the entity will trigger this same callback for the child
                world().entityDestroy(child);

                child = nextSibling;
            }

            // Cleanup root tag
            world().componentDestroy<HierarchyRootTag>(entity);
        }
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::game::HierarchyComponent);
RYN_TYPE_REFLECTION_KEEP(ryn::game::HierarchySystem);