module;
#include "Prelude.h"

export module Ryntime.System:Hierarchy;
export import Ryntime.Service;
export import Ryntime.Core.Reflection;

using namespace ryn::service;
using namespace ryn::reflection;

RYN_SCOPE_FILE("System|Hierarchy");

export namespace ryn::system {
    class Hierarchy;
}

export namespace ryn::comp {
    /**
     * @struct  ryn::comp::HierarchyParentRequest
     * @brief   Provides a way to request reparenting of an entity.
     *
     * This component forms a request to the hierarchy system to reparent it's respective entity.
     * To unparent an entity simply attach this component to it and pass NewParent Entity::Null.
     */
    struct HierarchyParentRequest : IComponentAction<"ryn::comp::HierarchyParentRequest"> {
        Entity NewParent = Entity::Null;
    };

    /**
     * @struct  ryn::comp::Hierarchy
     * @brief   Provides vertical traversal data for the scene hierarchy.
     *
     * This component defines the primary parent-child relationship. It is optimized
     * for top-down recursive updates (like Transform solving) by storing only the
     * parent and the head of the children list.
     *
     * @note    Horizontal traversal (iterating through all children of a parent)
     *          requires the @ref ryn::comp::HierarchySibling component.
     */
    struct Hierarchy : IComponent<"ryn::comp::Hierarchy"> {
        inline Entity getParent() const { return Parent; }
        inline Entity getFirstChild() const { return FirstChild; }

    private:
        friend system::Hierarchy;
        Entity Parent     = Entity::Null;
        Entity FirstChild = Entity::Null;
    };

    /**
     * @struct  ryn::comp::HierarchySibling
     * @brief   Provides horizontal traversal data for the scene hierarchy.
     *
     * This component forms a doubly-linked list among entities sharing the same parent.
     * It is used to navigate between siblings once a parent's FirstChild has been reached.
     */
    struct HierarchySibling : IComponent<"ryn::comp::HierarchySibling"> {
        inline Entity getNext() const { return Next; }
        inline Entity getPrev() const { return Prev; }

    private:
        friend system::Hierarchy;
        Entity Next = Entity::Null;
        Entity Prev = Entity::Null;
    };
}

export namespace ryn::system {
    class RYN_API Hierarchy final : public DynamicType<ISystem, "ryn::system::Hierarchy"> {
        WorldConnection m_OnHierarchyDestroy;
        WorldConnection m_OnSiblingDestroy;

        void onConfigure(ServiceDiscovery& discovery) override {
            m_OnHierarchyDestroy = m_World->Observer.onDestroy<comp::Hierarchy, &Hierarchy::onHierarchyDestroy>(this);
            m_OnSiblingDestroy   = m_World->Observer.onDestroy<comp::HierarchySibling, &Hierarchy::onSiblingDestroy>(this);
        }

        // When a hierarchy (component) is destroyed we must kill the associated entity's children and promote it to a hierarchy root
        void onHierarchyDestroy(Entity entity) {
            auto current = m_World->Component.get<comp::Hierarchy>(entity);

            // Kill children
            Entity walker = current->FirstChild;
            while (walker) {
                auto child       = m_World->Component.tryGet<comp::HierarchySibling>(walker);
                auto nextSibling = child ? child->Next : Entity::Null;

                // Destroying the child's entity causes the same onHierarchyDestroy callback to be triggered for it; effectively making
                // this a depth-first recursive destruction
                m_World->Entity.destroy(walker);

                walker = nextSibling;
            }

            // Unlink from current hierarchy (effectively makes root) Promote to hierarchy root
            unlink(entity);
        }

        // When a sibling (component) is destroyed we must patch the associated double-linked sibling chain and promote the entity to root
        void onSiblingDestroy(Entity entity) { unlink(entity); }

        void onUpdate(const FrameTime time) override {
            m_World->Query.foreach<comp::HierarchyParentRequest>([&](Entity entity, auto& request) {
                if (request.NewParent != Entity::Null) setParent(entity, request.NewParent);
                else setParent(entity, Entity::Null);
            });

            m_World->Component.clear<comp::HierarchyParentRequest>();
        }

        void setParent(Entity entity, Entity newParent) {
            RYN_SCOPE_LOCAL("System|Hierarchy::setParent");

            auto current = m_World->Component.ensure<comp::Hierarchy>(entity);
            if (newParent && current->Parent == newParent) {
                RYN_WARN("Entity {0} is already parented to entity {1}!", entity, newParent);
                return;
            }

            if (isAncestorOf(entity, newParent)) {
                RYN_WARN("Circular hierarchy detected! Cannot parent {0} to its own child {1}.", entity, newParent);
                return;
            }

            // Unlink from current hierarchy before setting new parent (effectively makes root)
            unlink(entity);
            if (newParent == Entity::Null) return; // newParent isn't a valid entity, assume the user simply wanted to make root

            // Attach to new parent
            auto parent         = m_World->Component.ensure<comp::Hierarchy>(newParent);
            auto currentSibling = m_World->Component.ensure<comp::HierarchySibling>(entity);

            Entity oldFirstChild = parent->FirstChild;

            current->Parent      = newParent;
            currentSibling->Next = oldFirstChild;
            currentSibling->Prev = Entity::Null;

            // If the newParent already had a old first child we need to insert ourself into it's silbing.Prev field
            if (oldFirstChild) m_World->Component.ensure<comp::HierarchySibling>(oldFirstChild)->Prev = entity;

            parent->FirstChild = entity;
        }

        void unlink(Entity entity) {
            auto current = m_World->Component.tryGet<comp::Hierarchy>(entity);
            if (!current || !current->Parent) return;

            // Fix parent's first pointer if needed
            if (auto parent = m_World->Component.tryGet<comp::Hierarchy>(current->Parent))
                if (parent->FirstChild == entity) {
                    auto siblings      = m_World->Component.tryGet<comp::HierarchySibling>(entity);
                    parent->FirstChild = siblings ? siblings->Next : Entity::Null;
                }

            // Zero-out our pointer to parent
            current->Parent = Entity::Null;

            // Fix siblings if needed
            if (auto siblings = m_World->Component.tryGet<comp::HierarchySibling>(entity)) {
                if (siblings->Prev)
                    if (auto prev = m_World->Component.tryGet<comp::HierarchySibling>(siblings->Prev)) prev->Next = siblings->Next;
                if (siblings->Next)
                    if (auto next = m_World->Component.tryGet<comp::HierarchySibling>(siblings->Next)) next->Prev = siblings->Prev;

                // Zero-out our pointers to sibligns
                siblings->Prev = Entity::Null;
                siblings->Next = Entity::Null;
            }
        }

        bool isAncestorOf(Entity ancestor, Entity entityToTest) {
            Entity walker = entityToTest;
            while (walker) {
                if (walker == ancestor) return true;
                auto current = m_World->Component.tryGet<comp::Hierarchy>(walker);
                walker       = current ? current->Parent : Entity::Null;
            }
            return false;
        }
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::comp::HierarchyParentRequest);
RYN_TYPE_REFLECTION_KEEP(ryn::comp::Hierarchy);
RYN_TYPE_REFLECTION_KEEP(ryn::comp::HierarchySibling);
RYN_TYPE_REFLECTION_KEEP(ryn::system::Hierarchy);