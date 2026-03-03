module;
#include "Prelude.h"

export module Ryntime.System:Hierarchy;
export import Ryntime.Service;
export import Ryntime.Core.Reflection;

using namespace ryn::service;
using namespace ryn::reflection;

export namespace ryn::system {
    class Hierarchy;
}

export namespace ryn::component {
    struct HierarchyRoot : IComponent<"ryn::component::HierarchyRoot"> {};

    struct Hierarchy : IComponent<"ryn::component::Hierarchy"> {
        inline Entity getParent() const { return Parent; }
        inline Entity getFirstChild() const { return First; }
        inline Entity getNextSibling() const { return Next; }
        inline Entity getPrevSibling() const { return Prev; }

    private:
        friend system::Hierarchy;
        Entity Parent = Entity::Null;
        Entity First  = Entity::Null;
        Entity Next   = Entity::Null;
        Entity Prev   = Entity::Null;
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::component::HierarchyRoot);
RYN_TYPE_REFLECTION_KEEP(ryn::component::Hierarchy);

RYN_SCOPE_FILE("System|Hierarchy");

export namespace ryn::system {
    class RYN_API Hierarchy final : public DynamicType<ISystem, "ryn::system::Hierarchy"> {
    public:
        static void setParent(World& world, Entity entity, Entity parent) {
            if (!entity) return;

            auto currentComp = world.Component.getOrMake<component::Hierarchy>(entity);
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
            auto* parentComp = world.Component.getOrMake<component::Hierarchy>(parent);

            Entity oldFirst     = parentComp->First;
            currentComp->Parent = parent;
            currentComp->Next   = oldFirst;
            currentComp->Prev   = Entity::Null;

            if (oldFirst) world.Component.get<component::Hierarchy>(oldFirst)->Prev = entity;

            parentComp->First = entity;

            // Remove root tag, no longer a root
            world.Component.destroy<component::HierarchyRoot>(entity);
        }

        static void detachFromHierarchy(World& world, Entity entity) {
            auto* current = world.Component.tryGet<component::Hierarchy>(entity);
            if (!current) return;

            world.Component.getOrMake<component::HierarchyRoot>(entity);
            if (!current->Parent) return;

            auto* parentComp = world.Component.tryGet<component::Hierarchy>(current->Parent);
            if (parentComp) {
                if (parentComp->First == entity) parentComp->First = current->Next;
                if (current->Prev) world.Component.get<component::Hierarchy>(current->Prev)->Next = current->Next;
                if (current->Next) world.Component.get<component::Hierarchy>(current->Next)->Prev = current->Prev;
            }

            current->Parent = Entity::Null;
            current->Next   = Entity::Null;
            current->Prev   = Entity::Null;
        }

        static bool isAncestorOf(World& world, Entity ancestor, Entity descendant) {
            Entity current = descendant;
            while (current) {
                if (current == ancestor) return true;
                component::Hierarchy* comp = world.Component.tryGet<component::Hierarchy>(current);
                current                    = comp ? comp->Parent : Entity::Null;
            }
            return false;
        }

    private:
        World::ObserverAPI::Connection m_OnRootMake;
        World::ObserverAPI::Connection m_OnHierarchyMake;
        World::ObserverAPI::Connection m_OnHierarchyDestroy;

        void onConfigure(ServiceDiscovery& discovery) override {
            m_OnRootMake         = m_World->Observer.onMake<component::HierarchyRoot, &Hierarchy::onRootMake>(this);
            m_OnHierarchyMake    = m_World->Observer.onMake<component::Hierarchy, &Hierarchy::onHierarchyMake>(this);
            m_OnHierarchyDestroy = m_World->Observer.onDestroy<component::Hierarchy, &Hierarchy::onHierarchyDestroy>(this);
        }

        void onRootMake(Entity entity) {
            // If wrong conditions, force root tag back off
            component::Hierarchy* comp = m_World->Component.tryGet<component::Hierarchy>(entity);
            if (!comp || comp->Parent) m_World->Component.destroy<component::HierarchyRoot>(entity);
        }

        void onHierarchyMake(Entity entity) {
            // New hierarchy components are root by default
            m_World->Component.getOrMake<component::HierarchyRoot>(entity);
        }

        void onHierarchyDestroy(Entity entity) {
            detachFromHierarchy(*m_World, entity);

            auto current = m_World->Component.get<component::Hierarchy>(entity);

            Entity child = current->First;
            while (child) {
                auto   childHierarchy = m_World->Component.tryGet<component::Hierarchy>(child);
                Entity nextSibling    = childHierarchy ? childHierarchy->Next : Entity::Null;

                // Destroying the entity will trigger this same callback for the child
                m_World->Entity.destroy(child);

                child = nextSibling;
            }

            // Cleanup root tag
            m_World->Component.destroy<component::HierarchyRoot>(entity);
        }
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::system::Hierarchy);