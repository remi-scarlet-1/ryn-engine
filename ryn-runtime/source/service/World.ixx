module;
#include "Prelude.h"
#include <cstdint>
#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>

export module Ryntime.Service:World;
export import Ryntime.Core.Reflection;
export import Ryntime.Core;

using namespace ryn::reflection;

namespace entt {
    template<typename T>
    requires IsType<T>
    struct type_hash<T> {
        static constexpr id_type value() noexcept { return static_cast<id_type>(T::TYPE_ID.value()); }
    };

    template<typename T>
    requires IsType<T>
    struct type_name<T> {
        static constexpr std::string_view value() noexcept { return {T::TYPE_NAME}; }
    };
}

namespace ryn::service {
    struct ComponentTag {};
}

export namespace ryn::service {
    class World;

    struct Entity {
        static const Entity Null;

        constexpr Entity(entt::entity e) noexcept : id(static_cast<ENTT_ID_TYPE>(e)) {}
        constexpr operator entt::entity() const noexcept { return static_cast<entt::entity>(id); }
        constexpr operator ENTT_ID_TYPE() const noexcept { return id; }

        auto               operator<=>(const Entity&) const = default;
        explicit constexpr operator bool() const noexcept { return id != static_cast<ENTT_ID_TYPE>(entt::null); }

    private:
        ENTT_ID_TYPE id = entt::null;
    };

    constexpr Entity Entity::Null{static_cast<entt::entity>(entt::null)};

    template<TypeName Name>
    struct IComponent : StaticType<TypeCategory::Component, Name>, ComponentTag {};

    template<typename T>
    concept IsComponent = std::derived_from<T, ComponentTag> && IsType<T>;

    class RYN_API ServiceDiscovery : public IServiceLocator {
    public:
        explicit ServiceDiscovery(IServiceLocator& locator);
        [[nodiscard]] inline bool wasSuccess() const noexcept { return m_Success; }

    private:
        IServiceLocator& m_ServiceLocator;
        bool             m_Success = true;

        IService* requestService(TypeID typeId, bool required) override;
        void      notifyMissingService(TypeID typeId) override;
    };

    class RYN_API ISystem : public DynamicBase<TypeCategory::System> {
    public:
        ~ISystem() override;

        virtual inline void onConfigure([[maybe_unused]] ServiceDiscovery& discovery) {}
        virtual inline void onEnable() {}
        virtual inline void onDisable() {}

        virtual inline void onBeginFrame([[maybe_unused]] const FrameTime time, [[maybe_unused]] World& world) {}
        virtual inline void onUpdate([[maybe_unused]] const FrameTime time, [[maybe_unused]] World& world) {}
        virtual inline void onEndFrame([[maybe_unused]] const FrameTime time, [[maybe_unused]] World& world) {}
    };

    template<typename T>
    concept IsSystem = std::derived_from<T, ISystem> && IsType<T>;
}

namespace ryn::service {
    export using ObserverConnection = entt::scoped_connection;

    // clang-format off
    export template<typename... T> requires(IsComponent<T> && ...) struct Incl {};
    export template<typename... T> requires(IsComponent<T> && ...) struct Excl {};

    template<typename T> struct is_incl_tag : std::false_type {};
    template<typename... Ts> struct is_incl_tag<Incl<Ts...>> : std::true_type {};
    template<typename T> struct is_excl_tag : std::false_type {};
    template<typename... Ts> struct is_excl_tag<Excl<Ts...>> : std::true_type {};

    template<typename T> concept IsInclTag = is_incl_tag<T>::value;
    template<typename T> concept IsExclTag = is_excl_tag<T>::value;
    // clang-format on
}

export namespace ryn::service {
    class RYN_API World final : public DynamicType<IService, "ryn::service::World"> {
    public:
        // clang-format off
        [[nodiscard]] inline                      Entity                entityCreate();
        inline                                    void                  entityDestroy(Entity entity);

        template<IsComponent T, typename... Args> T*                    componentMake(Entity entity, Args&&... args);
        template<IsComponent T, typename... Args> T*                    componentGetOrMake(Entity entity, Args&&... args);
        template<IsComponent T>                   [[nodiscard]] T*      componentGet(Entity entity);
        template<IsComponent T>                   [[nodiscard]] T*      componentTryGet(Entity entity);
        template<IsComponent T>                   [[nodiscard]] bool    componentExists(Entity entity);
        template<IsComponent T>                   bool                  componentDestroy(Entity entity);

        template<IsSystem T, typename... Args>    T*                    systemMake(Args&&... args);
        template<IsSystem T>                      bool                  systemDestroy();
        template<IsSystem T>                      bool                  systemToggle(bool enable);

        template<IsComponent T, typename... Args> T*                    singletonMake(Args&&... args);
        template<IsComponent T, typename... Args> T*                    singletonGetOrMake(Args&&... args);
        template<IsComponent T>                   [[nodiscard]] T*      singletonGet();
        template<IsComponent T>                   [[nodiscard]] T*      singletonTryGet();
        template<IsComponent T>                   [[nodiscard]] bool    singletonExists();
        template<IsComponent T>                   bool                  singletonDestroy();

        template<IsComponent T>                                          [[nodiscard]] Span<T>  all();
        template<IsInclTag Incl, IsExclTag Excl = Excl<>, typename Func> void                   foreach(Func&& func);

        // Signature for all callback Candidates must be: void(entt::registry&, entt::entity)
        template<IsComponent T, auto Candidate>                void                             onComponentMade();
        template<IsComponent T, auto Candidate>                void                             onComponentDestroyed();
        template<IsComponent T, auto Candidate>                void                             offComponentMade();
        template<IsComponent T, auto Candidate>                void                             offComponentDestroyed();
        template<IsComponent T, auto Candidate, typename Type> [[nodiscard]] ObserverConnection onComponentMade(Type* instance);
        template<IsComponent T, auto Candidate, typename Type> [[nodiscard]] ObserverConnection onComponentDestroyed(Type* instance);
        template<IsComponent T, auto Candidate, typename Type> void                             offComponentMade(Type* instance);
        template<IsComponent T, auto Candidate, typename Type> void                             offComponentDestroyed(Type* instance);
        // clang-format on

    protected:
        void onAttach(IServiceLocator& locator) override;
        void onDetach() override;
        void onUpdate(const FrameTime& time) override;

    private:
        IServiceLocator* m_ServiceLocator = nullptr;
        entt::registry   m_Registry;

        struct SystemEntry {
            Box<ISystem> Instance;
            TypeID       CachedTypeID;
            bool         IsEnabled;
            bool         IsConfigured;
        };

        std::vector<SystemEntry>           m_Systems;
        std::unordered_map<TypeID, size_t> m_SystemIndexMap;

        bool internal_systemMake(TypeID id, Box<ISystem>&& system);
        bool internal_systemDestroy(TypeID id);
        bool internal_systemToggle(TypeID id, bool enable);
        void sweepSystems();
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::service::World);

/* ===== Inline API Implementations ===== */
namespace ryn::service {
    Entity World::entityCreate() { return m_Registry.create(); }

    void World::entityDestroy(Entity entity) { m_Registry.destroy(entity); }

    template<IsComponent T, typename... Args>
    T* World::componentMake(Entity entity, Args&&... args) {
        return &m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

    template<IsComponent T, typename... Args>
    T* World::componentGetOrMake(Entity entity, Args&&... args) {
        return &m_Registry.get_or_emplace<T>(entity, std::forward<Args>(args)...);
    }

    template<IsComponent T>
    T* World::componentGet(Entity entity) {
        return &m_Registry.get<T>(entity);
    }

    template<IsComponent T>
    T* World::componentTryGet(Entity entity) {
        return m_Registry.try_get<T>(entity);
    }

    template<IsComponent T>
    bool World::componentExists(Entity entity) {
        return m_Registry.all_of<T>(entity);
    }

    template<IsComponent T>
    bool World::componentDestroy(Entity entity) {
        return m_Registry.remove<T>(entity) > 0;
    }

    template<IsSystem T, typename... Args>
    T* World::systemMake(Args&&... args) {
        Box<ISystem> systemBox = Box<T>::template make<T>(std::forward<Args>(args)...);
        ISystem*     system    = systemBox.get();
        return internal_systemMake(T::TYPE_ID, systemBox) ? static_cast<T*>(system) : nullptr;
    }

    template<IsSystem T>
    bool World::systemDestroy() {
        return internal_systemDestroy(T::TYPE_ID);
    }

    template<IsSystem T>
    bool World::systemToggle(bool enable) {
        return internal_systemToggle(T::TYPE_ID, enable);
    }

    template<IsComponent T, typename... Args>
    T* World::singletonMake(Args&&... args) {
        return &m_Registry.ctx().emplace<T>(std::forward<Args>(args)...);
    }

    template<IsComponent T, typename... Args>
    T* World::singletonGetOrMake(Args&&... args) {
        auto& ctx = m_Registry.ctx();
        if (T* ptr = ctx.find<T>()) return ptr;
        return &ctx.emplace<T>(std::forward<Args>(args)...);
    }

    template<IsComponent T>
    T* World::singletonGet() {
        return &m_Registry.ctx().get<T>();
    }

    template<IsComponent T>
    T* World::singletonTryGet() {
        return m_Registry.ctx().find<T>();
    }

    template<IsComponent T>
    bool World::singletonExists() {
        return m_Registry.ctx().contains<T>();
    }

    template<IsComponent T>
    bool World::singletonDestroy() {
        return m_Registry.ctx().erase<T>();
    }

    template<IsComponent T>
    Span<T> World::all() {
        auto& storage = m_Registry.storage<T>();
        return Span<T>{storage.data(), storage.size()};
    }

    template<typename InclTag, typename ExclTag>
    struct ViewExtractor;

    template<typename... I, typename... E>
    struct ViewExtractor<Incl<I...>, Excl<E...>> {
        static auto get(entt::registry& reg) { return reg.view<I...>(entt::exclude<E...>); }
    };

    template<IsInclTag Incl, IsExclTag Excl, typename Func>
    void World::foreach (Func&& func) {
        auto view = ViewExtractor<Incl, Excl>::get(m_Registry);
        view.each(std::forward<Func>(func));
    }

    template<IsComponent T, auto Candidate>
    void World::onComponentMade() {
        m_Registry.on_construct<T>().template connect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::onComponentDestroyed() {
        m_Registry.on_destroy<T>().template connect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::offComponentMade() {
        m_Registry.on_construct<T>().template disconnect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::offComponentDestroyed() {
        m_Registry.on_destroy<T>().template disconnect<Candidate>();
    }

    template<IsComponent T, auto Candidate, typename Type>
    ObserverConnection World::onComponentMade(Type* instance) {
        return m_Registry.on_construct<T>().template connect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    ObserverConnection World::onComponentDestroyed(Type* instance) {
        return m_Registry.on_destroy<T>().template connect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    void World::offComponentMade(Type* instance) {
        m_Registry.on_construct<T>().template disconnect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    void World::offComponentDestroyed(Type* instance) {
        m_Registry.on_destroy<T>().template disconnect<Candidate>(instance);
    }
}