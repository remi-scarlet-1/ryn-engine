module;
#include "Prelude.h"
#include <cstdint>
#include <entt/entt.hpp>
#include <format>
#include <unordered_map>
#include <vector>

export module Ryntime.Service.World:World;
export import Ryntime.Reflect;
export import Ryntime.Core;

RYN_SCOPE_FILE("Service|World");

namespace entt {
    template<typename T>
    requires ryn::IsType<T>
    struct type_hash<T> {
        static constexpr id_type value() noexcept { return static_cast<id_type>(T::TYPE_ID.Hash); }
    };

    template<typename T>
    requires ryn::IsType<T>
    struct type_name<T> {
        static constexpr std::string_view value() noexcept { return {T::TYPE_NAME}; }
    };
}

namespace ryn::service {
    struct ComponentMarker {};
}

export namespace ryn::service {
    class World;

    struct RYN_API Entity {
        using IDType = ENTT_ID_TYPE;

        constexpr Entity(entt::entity e) noexcept : id(static_cast<IDType>(e)) {}
        constexpr operator entt::entity() const noexcept { return static_cast<entt::entity>(id); }
        constexpr operator IDType() const noexcept { return id; }

        auto               operator<=>(const Entity&) const = default;
        explicit constexpr operator bool() const noexcept { return id != static_cast<IDType>(entt::null); }

        static const Entity Null;

    private:
        IDType id = entt::null;
    };

    constexpr Entity Entity::Null{static_cast<entt::entity>(entt::null)};

    template<TypeName Name>
    struct IComponent : StaticType<TypeCategory::Component, Name>, ComponentMarker {};

    template<TypeName Name>
    struct IComponentRequest : StaticType<TypeCategory::ComponentRequest, Name>, ComponentMarker {};

    template<typename T>
    concept IsComponent = std::derived_from<T, ComponentMarker> && IsType<T>;

    class RYN_API ISystem : public DynamicBase<TypeCategory::System> {
    public:
        ~ISystem() override;

        virtual inline void onConfigure([[maybe_unused]] ServiceDiscovery& discovery) {}
        virtual inline void onEnable() {}
        virtual inline void onDisable() {}

        virtual inline void onBeginFrame([[maybe_unused]] const Time time) {}
        virtual inline void onUpdate([[maybe_unused]] const Time time) {}
        virtual inline void onEndFrame([[maybe_unused]] const Time time) {}

    protected:
        friend World;
        World* m_World = nullptr;
    };

    template<typename T>
    concept IsSystem = std::derived_from<T, ISystem> && IsType<T>;
}

template<>
struct std::formatter<ryn::service::Entity> {
    constexpr auto parse(const std::format_parse_context& ctx) const { return ctx.begin(); }

    inline auto format(const ryn::service::Entity entity, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "[ID: {}]", static_cast<ENTT_ID_TYPE>(entity));
    }
};

template<>
struct std::hash<ryn::service::Entity> {
    size_t operator()(const ryn::service::Entity& e) const noexcept { return std::hash<ryn::service::Entity::IDType>{}(e); }
};

namespace ryn::service {
    // clang-format off
    export using                    ObserverConnection = entt::scoped_connection;
    export template<IsComponent...> struct QueryIncl {};
    export template<IsComponent...> struct QueryExcl {};

    template<typename>              struct is_query_incl                  : std::false_type {};
    template<IsComponent... I>      struct is_query_incl<QueryIncl<I...>> : std::true_type {};
    template<typename>              struct is_query_excl                  : std::false_type {};
    template<IsComponent... E>      struct is_query_excl<QueryExcl<E...>> : std::true_type {};
    // clang-format on
}

export namespace ryn::service {
    class RYN_API World final : public DynamicType<IService, "ryn::service::World"> {
    public:
        // clang-format off
        struct EntityAPI {
            World& self;
            [[nodiscard]] inline Entity create();
            inline void                 destroy(Entity entity);
        };

        struct SystemAPI {
            World& self;
            template<IsSystem T, typename... Args> T*   make(Args&&... args);
            template<IsSystem T>                   bool destroy();
            template<IsSystem T>                   bool toggle(bool enable);
        };

        struct ComponentAPI {
            World& self;
            template<IsComponent T> T*                 add(Entity entity);
            template<IsComponent T> T*                 add(Entity entity, T newData);
            template<IsComponent T> T*                 ensure(Entity entity);
            template<IsComponent T> T*                 ensure(Entity entity, T newData);
            template<IsComponent T> T*                 replace(Entity entity);
            template<IsComponent T> T*                 replace(Entity entity, T newData);
            template<IsComponent T> T*                 set(Entity entity);
            template<IsComponent T> T*                 set(Entity entity, T newData);
            template<IsComponent T, typename Func> T*  patch(Entity entity, Func&& func); // Use with lambdas like [](auto& c) { ... }
            template<IsComponent T>                T*  patch(Entity entity);              // Just triggers onUpdate for the component
            template<IsComponent T> [[nodiscard]] T*   get(Entity entity);
            template<IsComponent T> [[nodiscard]] T*   tryGet(Entity entity);
            template<IsComponent T> [[nodiscard]] bool exists(Entity entity);
            template<IsComponent T> bool               destroy(Entity entity);
            template<IsComponent T> void               clear();
        };

        struct SingletonAPI {
            World& self;
            template<IsComponent T, typename... Args> T*                 make(Args&&... args);
            template<IsComponent T, typename... Args> T*                 getOrMake(Args&&... args);
            template<IsComponent T>                   [[nodiscard]] T*   get();
            template<IsComponent T>                   [[nodiscard]] T*   tryGet();
            template<IsComponent T>                   [[nodiscard]] bool exists();
            template<IsComponent T>                   bool               destroy();
        };


        struct ObserverAPI {
            World& self;
            template<IsComponent T, auto Candidate>                void                             onMake();
            template<IsComponent T, auto Candidate>                void                             onUpdate();
            template<IsComponent T, auto Candidate>                void                             onDestroy();
            template<IsComponent T, auto Candidate, typename Type> [[nodiscard]] ObserverConnection onMake(Type* instance);
            template<IsComponent T, auto Candidate, typename Type> [[nodiscard]] ObserverConnection onUpdate(Type* instance);
            template<IsComponent T, auto Candidate, typename Type> [[nodiscard]] ObserverConnection onDestroy(Type* instance);
            template<IsComponent T, auto Candidate>                void                             offMake();
            template<IsComponent T, auto Candidate>                void                             offUpdate();
            template<IsComponent T, auto Candidate>                void                             offDestroy();
            template<IsComponent T, auto Candidate, typename Type> void                             offMake(Type* instance);
            template<IsComponent T, auto Candidate, typename Type> void                             offUpdate(Type* instance);
            template<IsComponent T, auto Candidate, typename Type> void                             offDestroy(Type* instance);
        };

        struct QueryAPI {
            World& self;
            template<IsComponent T>                                    [[nodiscard]] Span<T>  all();
            template<typename T1, typename T2 = QueryExcl<>, typename Func> void              foreach(Func&& func);

        private:
            template<IsComponent... I, IsComponent... E, typename Func>
            void executeForeach(QueryIncl<I...>, QueryExcl<E...>, Func&& func);
        };
        // clang-format on

        friend EntityAPI;
        friend SystemAPI;
        friend ComponentAPI;
        friend SingletonAPI;
        friend ObserverAPI;
        friend QueryAPI;

        EntityAPI    Entity    = EntityAPI{*this};
        SystemAPI    System    = SystemAPI{*this};
        ComponentAPI Component = ComponentAPI{*this};
        SingletonAPI Singleton = SingletonAPI{*this};
        ObserverAPI  Observer  = ObserverAPI{*this};
        QueryAPI     Query     = QueryAPI{*this};

    private:
        struct SystemEntry {
            Box<ISystem> Instance;
            TypeID       Id;
            bool         IsEnabled;
            bool         IsConfigured;
        };

        IServiceLocator*                   m_ServiceLocator = nullptr;
        entt::registry                     m_Registry;
        std::vector<SystemEntry>           m_Systems;
        std::unordered_map<TypeID, size_t> m_SystemIndexMap;

        void onAttach(IServiceLocator& locator) override;
        void onDetach() override;
        void onUpdate(const Time& time) override;

        bool makeSystem(TypeID id, Box<ISystem>&& system);
        bool destroySystem(TypeID id);
        bool toggleSystem(TypeID id, bool enable);
        void sweepSystems();
    };
}

RYN_KEEP_REFLECTED_TYPE(ryn::service::World);

/* ===== Inline API Implementations ===== */
namespace ryn::service {
    // Entt helper required to wrap registry calls that return a pointer to a components.
    // In case the component is a zero size struct (e.g. a simple tag) which get's optimized by entt to void.
    template<typename T>
    T* entt_ptr_call(auto&& func) {
        if constexpr (std::is_empty_v<T>) {
            func(); // Call void function
            return nullptr;
        } else {
            return &func(); // Call T& function, return address
        }
    }

    Entity World::EntityAPI::create() { return self.m_Registry.create(); }

    void World::EntityAPI::destroy(service::Entity entity) { self.m_Registry.destroy(entity); }

    template<IsSystem T, typename... Args>
    T* World::SystemAPI::make(Args&&... args) {
        Box<ISystem> systemBox = Box<T>::template make<T>(std::forward<Args>(args)...);
        ISystem*     system    = systemBox.get();
        system->m_World        = &self;
        return self.makeSystem(T::TYPE_ID, std::move(systemBox)) ? static_cast<T*>(system) : nullptr;
    }

    template<IsSystem T>
    bool World::SystemAPI::destroy() {
        return self.destroySystem(T::TYPE_ID);
    }

    template<IsSystem T>
    bool World::SystemAPI::toggle(bool enable) {
        return self.toggleSystem(T::TYPE_ID, enable);
    }

    template<IsComponent T>
    T* World::ComponentAPI::add(service::Entity entity) {
        RYN_ASSERT(!exists<T>(entity), "Expected entity {0} to NOT have a '{1}' component!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.emplace<T>(entity); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::add(service::Entity entity, T newData) {
        RYN_ASSERT(!exists<T>(entity), "Expected entity {0} to NOT have a '{1}' component!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.emplace<T>(entity, std::move(newData)); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::ensure(service::Entity entity) {
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.get_or_emplace<T>(entity); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::ensure(service::Entity entity, T newData) {
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.get_or_emplace<T>(entity, std::move(newData)); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::replace(service::Entity entity) {
        RYN_ASSERT(exists<T>(entity), "Expected entity {0} to have a '{1}' component!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.replace<T>(entity); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::replace(service::Entity entity, T newData) {
        RYN_ASSERT(exists<T>(entity), "Expected entity {0} to have a '{1}' component!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.replace<T>(entity, std::move(newData)); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::set(service::Entity entity) {
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.emplace_or_replace<T>(entity); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::set(service::Entity entity, T newData) {
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.emplace_or_replace<T>(entity, std::move(newData)); });
    }

    template<IsComponent T, typename Func>
    T* World::ComponentAPI::patch(service::Entity entity, Func&& func) {
        RYN_ASSERT(exists<T>(entity), "Expected entity {0} to have a '{1}' component to patch!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.patch<T>(entity, std::forward<Func>(func)); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::patch(service::Entity entity) {
        RYN_ASSERT(exists<T>(entity), "Expected entity {0} to have a '{1}' component to patch!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.patch<T>(entity); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::get(service::Entity entity) {
        RYN_ASSERT(exists<T>(entity), "Expected entity {0} to have a '{1}' component!", entity, T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.get<T>(entity); });
    }

    template<IsComponent T>
    T* World::ComponentAPI::tryGet(service::Entity entity) {
        if constexpr (std::is_empty_v<T>) return nullptr;
        else return self.m_Registry.try_get<T>(entity);
    }

    template<IsComponent T>
    bool World::ComponentAPI::exists(service::Entity entity) {
        return self.m_Registry.all_of<T>(entity);
    }

    template<IsComponent T>
    bool World::ComponentAPI::destroy(service::Entity entity) {
        return self.m_Registry.remove<T>(entity) > 0;
    }

    template<IsComponent T>
    void World::ComponentAPI::clear() {
        self.m_Registry.clear<T>();
    }

    template<IsComponent T, typename... Args>
    T* World::SingletonAPI::make(Args&&... args) {
        RYN_ASSERT(!exists<T>(), "Expected singleton of type '{0}' to NOT already exist!", T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.ctx().emplace<T>(std::forward<Args>(args)...); });
    }

    template<IsComponent T, typename... Args>
    T* World::SingletonAPI::getOrMake(Args&&... args) {
        if (T* ptr = tryGet<T>()) return ptr;
        return make<T>(std::forward<Args>(args)...);
    }

    template<IsComponent T>
    T* World::SingletonAPI::get() {
        RYN_ASSERT(exists<T>(), "Expected singleton of type '{0}' to already exist!", T::TYPE_NAME);
        return entt_ptr_call<T>([&] -> decltype(auto) { return self.m_Registry.ctx().get<T>(); });
    }

    template<IsComponent T>
    T* World::SingletonAPI::tryGet() {
        if constexpr (std::is_empty_v<T>) return nullptr;
        else return self.m_Registry.ctx().find<T>();
    }

    template<IsComponent T>
    bool World::SingletonAPI::exists() {
        return self.m_Registry.ctx().contains<T>();
    }

    template<IsComponent T>
    bool World::SingletonAPI::destroy() {
        return self.m_Registry.ctx().erase<T>();
    }

    template<IsComponent T, auto Candidate>
    void World::ObserverAPI::onMake() {
        self.m_Registry.on_construct<T>().template connect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::ObserverAPI::onUpdate() {
        self.m_Registry.on_update<T>().template connect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::ObserverAPI::onDestroy() {
        self.m_Registry.on_destroy<T>().template connect<Candidate>();
    }

    template<IsComponent T, auto Candidate, typename Type>
    ObserverConnection World::ObserverAPI::onMake(Type* instance) {
        return self.m_Registry.on_construct<T>().template connect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    ObserverConnection World::ObserverAPI::onUpdate(Type* instance) {
        return self.m_Registry.on_update<T>().template connect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    ObserverConnection World::ObserverAPI::onDestroy(Type* instance) {
        return self.m_Registry.on_destroy<T>().template connect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate>
    void World::ObserverAPI::offMake() {
        self.m_Registry.on_construct<T>().template disconnect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::ObserverAPI::offUpdate() {
        self.m_Registry.on_update<T>().template disconnect<Candidate>();
    }

    template<IsComponent T, auto Candidate>
    void World::ObserverAPI::offDestroy() {
        self.m_Registry.on_destroy<T>().template disconnect<Candidate>();
    }

    template<IsComponent T, auto Candidate, typename Type>
    void World::ObserverAPI::offMake(Type* instance) {
        self.m_Registry.on_construct<T>().template disconnect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    void World::ObserverAPI::offUpdate(Type* instance) {
        self.m_Registry.on_update<T>().template disconnect<Candidate>(instance);
    }

    template<IsComponent T, auto Candidate, typename Type>
    void World::ObserverAPI::offDestroy(Type* instance) {
        self.m_Registry.on_destroy<T>().template disconnect<Candidate>(instance);
    }

    template<IsComponent T>
    Span<T> World::QueryAPI::all() {
        auto& storage = self.m_Registry.storage<T>();
        return Span<T>{storage.data(), storage.size()};
    }

    template<typename T1, typename T2 = QueryExcl<>, typename Func>
    void World::QueryAPI::foreach (Func&& func) {
        if constexpr (IsComponent<T1>) {
            // Case: T1 is a single component
            executeForeach(QueryIncl<T1>{}, T2{}, std::forward<Func>(func));
        } else if constexpr (is_query_excl<T1>::value) {
            // Case: T1 is an Excl tag (assume Inclusion is empty)
            executeForeach(QueryIncl{}, T1{}, std::forward<Func>(func));
        } else {
            // Standard Case: T1 is Incl, T2 is Excl
            executeForeach(T1{}, T2{}, std::forward<Func>(func));
        }
    }

    template<IsComponent... I, IsComponent... E, typename Func>
    void World::QueryAPI::executeForeach(QueryIncl<I...>, QueryExcl<E...>, Func&& func) {
        // Create the EnTT view based on whether we have inclusions or just exclusions
        auto view = [&] {
            if constexpr (sizeof...(I) == 0) return self.m_Registry.view<entt::entity>(entt::exclude<E...>);
            else return self.m_Registry.view<I...>(entt::exclude<E...>);
        }();

        view.each(std::forward<Func>(func));
    }
}