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
    struct Hierarchy : IComponent<"ryn::component::Hierarchy"> {
        inline Entity getParent() const { return Parent; }
        inline Entity getFirstChild() const { return FirstChild; }

    private:
        friend system::Hierarchy;
        Entity Parent     = Entity::Null;
        Entity FirstChild = Entity::Null;
    };

    struct HierarchySibling : IComponent<"ryn.component::HierarchySibling"> {
        inline Entity getNext() const { return Next; }
        inline Entity getPrev() const { return Prev; }

    private:
        friend system::Hierarchy;
        Entity Next = Entity::Null;
        Entity Prev = Entity::Null;
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::component::Hierarchy);
RYN_TYPE_REFLECTION_KEEP(ryn::component::HierarchySibling);

RYN_SCOPE_FILE("System|Hierarchy");

export namespace ryn::system {
    class RYN_API Hierarchy final : public DynamicType<ISystem, "ryn::system::Hierarchy"> {
    public:
        static void setParent(World& world, Entity entity, Entity newParent) {
            RYN_SCOPE_LOCAL("System|Hierarchy::setParent");

            if (!entity) {
                RYN_WARN("Entity {0} passed isn't valid!", entity);
                return;
            }

            auto current = world.Component.getOrMake<component::Hierarchy>(entity);
            if (current->Parent == newParent) {
                RYN_WARN("Entity {0} is already parented to entity {1}!", entity, newParent);
                return;
            }

            if (isAncestorOf(world, entity, newParent)) {
                RYN_WARN("Circular hierarchy detected! Cannot parent {0} to it's own child {1}.", entity, newParent);
                return;
            }

            // Clear parent before setting new one
            clearParent(world, entity);
            if (newParent == Entity::Null) return; // newParent isn't a valid entity, assume the user simply wanted to clear current parent

            // Attach to new parent
            auto parent         = world.Component.getOrMake<component::Hierarchy>(newParent);
            auto currentSibling = world.Component.getOrMake<component::HierarchySibling>(entity);

            Entity oldFirstChild = parent->FirstChild;

            current->Parent      = newParent;
            currentSibling->Next = oldFirstChild;
            currentSibling->Prev = Entity::Null;

            // If the newParent already had a old first child we need to insert ourself into it's silbing.Prev field
            if (oldFirstChild) world.Component.get<component::HierarchySibling>(oldFirstChild)->Prev = entity;

            parent->FirstChild = entity;
        }

        static void clearParent(World& world, Entity entity) {
            auto current = world.Component.tryGet<component::Hierarchy>(entity);
            if (!current || !current->Parent) return;

            // Fix parent's first pointer if needed
            if (auto parent = world.Component.tryGet<component::Hierarchy>(current->Parent))
                if (parent->FirstChild == entity) {
                    auto siblings      = world.Component.tryGet<component::HierarchySibling>(entity);
                    parent->FirstChild = siblings ? siblings->Next : Entity::Null;
                }

            // Zero-out our pointer to parent
            current->Parent = Entity::Null;

            // Fix siblings if needed
            if (auto siblings = world.Component.tryGet<component::HierarchySibling>(entity)) {
                if (siblings->Prev)
                    if (auto prev = world.Component.tryGet<component::HierarchySibling>(siblings->Prev)) prev->Next = siblings->Next;
                if (siblings->Next)
                    if (auto next = world.Component.tryGet<component::HierarchySibling>(siblings->Next)) next->Prev = siblings->Prev;

                // Zero-out our pointers to sibligns
                siblings->Prev = Entity::Null;
                siblings->Next = Entity::Null;
            }
        }

        static bool isAncestorOf(World& world, Entity ancestor, Entity entityToTest) {
            Entity walker = entityToTest;
            while (walker) {
                if (walker == ancestor) return true;
                auto current = world.Component.tryGet<component::Hierarchy>(walker);
                walker       = current ? current->Parent : Entity::Null;
            }
            return false;
        }

    private:
        World::ObserverAPI::Connection m_OnHierarchyDestroy;

        void onConfigure(ServiceDiscovery& discovery) override {
            m_OnHierarchyDestroy = m_World->Observer.onDestroy<component::Hierarchy, &Hierarchy::onHierarchyDestroy>(this);
        }

        // When a hierarchy (component) is destroyed we must kill the associated entity's children and promote it to a hierarchy root
        void onHierarchyDestroy(Entity entity) {
            auto current = m_World->Component.get<component::Hierarchy>(entity);

            // Kill children
            Entity walker = current->FirstChild;
            while (walker) {
                auto child       = m_World->Component.tryGet<component::HierarchySibling>(walker);
                auto nextSibling = child ? child->Next : Entity::Null;

                // Destroying the child's entity causes the same onHierarchyDestroy callback to be triggered for it; effectively making
                // this a depth-first recursive destruction
                m_World->Entity.destroy(walker);

                walker = nextSibling;
            }

            // Promote to hierarchy root
            clearParent(*m_World, entity);
        }
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::system::Hierarchy);