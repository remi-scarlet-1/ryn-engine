module;
#include "Prelude.h"
#include <cstdint>
#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>

export module Ryntime.World:World;
export import Ryntime.Core;
export import :Entity;
export import :Component;
export import :Processor;

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

namespace ryn::world {
    export class World;

    // clang-format off
    struct EntityAPI {
        EntityAPI(World& world) : World(world) {}
        [[nodiscard]] inline Entity create();
        inline void                 destroy(Entity entity);

    private:
        World& World;
    };

    struct ComponentAPI {
        ComponentAPI(World& world) : World(world) {}
        template<typename T, typename... Args> requires IsComponent<T> T& make(Entity entity, Args&&... args);
        template<typename T, typename... Args> requires IsComponent<T> T& getOrMake(Entity entity, Args&&... args);
        template<typename T> requires IsComponent<T> [[nodiscard]] T&     get(Entity entity);
        template<typename T> requires IsComponent<T> [[nodiscard]] T*     tryGet(Entity entity);
        template<typename T> requires IsComponent<T> [[nodiscard]] bool   has(Entity entity) const;
        template<typename T> requires IsComponent<T> bool                 destroy(Entity entity);

    private:
        World& World;
    };

    struct ProcessorAPI {
        ProcessorAPI(World& world) : World(world) {}
        template<typename T, typename... Args> requires IsProcessor<T> T* make(Args&&... args);
        template<typename T> requires IsProcessor<T> bool                 toggle(bool enable);
        template<typename T> requires IsProcessor<T> bool                 destroy();

    private:
        World& World;
    };

    struct GlobalAPI {
        GlobalAPI(World& world) : World(world) {}
        template<typename T, typename... Args> requires IsComponent<T> T& make(Args&&... args);
        template<typename T, typename... Args> requires IsComponent<T> T& getOrMake(Args&&... args);
        template<typename T> requires IsComponent<T> [[nodiscard]] T&     get();
        template<typename T> requires IsComponent<T> [[nodiscard]] bool   has();
        template<typename T> requires IsComponent<T> bool                 destroy();

    private:
        World& World;
    };

    export using ObserverConnection = entt::scoped_connection;

    struct ObserverAPI {
        ObserverAPI(World& world) : World(world) {}
        template<typename T, auto Candidate> requires IsComponent<T> void onComponentMake();
        template<typename T, auto Candidate> requires IsComponent<T> void onComponentDestroy();
        template<typename T, auto Candidate> requires IsComponent<T> void offComponentMake();
        template<typename T, auto Candidate> requires IsComponent<T> void offComponentDestroy();
        template<typename T, auto Candidate, typename Type> requires IsComponent<T> ObserverConnection onComponentMake(Type* inst);
        template<typename T, auto Candidate, typename Type> requires IsComponent<T> ObserverConnection onComponentDestroy(Type* inst);
        template<typename T, auto Candidate, typename Type> requires IsComponent<T> void               offComponentMake(Type* inst);
        template<typename T, auto Candidate, typename Type> requires IsComponent<T> void               offComponentDestroy(Type* inst);

    private:
        World& World;
    };

    export template<typename... T> requires (IsComponent<T> && ...) struct Incl {};
    export template<typename... T> requires (IsComponent<T> && ...) struct Excl {};

    template<typename T>     struct is_incl_tag              : std::false_type {};
    template<typename... Ts> struct is_incl_tag<Incl<Ts...>> : std::true_type {};
    template<typename T>     struct is_excl_tag              : std::false_type {};
    template<typename... Ts> struct is_excl_tag<Excl<Ts...>> : std::true_type {};

    template<typename T> concept IsInclusionTag = is_incl_tag<T>::value;
    template<typename T> concept IsExclusionTag = is_excl_tag<T>::value;

    struct ViewAPI {
        ViewAPI(World& world) : World(world) {}
        template<typename T> requires IsComponent<T> [[nodiscard]] Span<T> all();
        template<IsInclusionTag Inclusion, IsExclusionTag Exclusion = Excl<>, typename Func> void foreach(Func&& func);

    private:
        World& World;
        template<typename... I, typename... E, typename Func> void foreachImplementation(Incl<I...>, Excl<E...>, Func&& func);
    };
    // clang-format on
}

export namespace ryn::world {
    class RYN_API World final : public DynamicType<IService, "ryn::world::World"> {
    public:
        World() : Entity(*this), Component(*this), Processor(*this), Global(*this), Observer(*this), View(*this) {}
        ~World() override {}

        EntityAPI    Entity;
        ComponentAPI Component;
        ProcessorAPI Processor;
        GlobalAPI    Global;
        ObserverAPI  Observer;
        ViewAPI      View;

        friend EntityAPI;
        friend ComponentAPI;
        friend ProcessorAPI;
        friend GlobalAPI;
        friend ObserverAPI;
        friend ViewAPI;

    protected:
        void onAttach(IServiceLocator& locator) override;
        void onDetach() override;
        void onUpdate(const FrameTime& time) override;

    private:
        struct ProcessorDetails {
            IProcessor* Instance;
            bool        IsEnabled;
        };

        IServiceLocator* m_ServiceLocator = nullptr;
        entt::registry   m_Registry;

        std::unordered_map<TypeID, ProcessorDetails> m_ProcessorLUT;
        std::vector<Box<IProcessor>>                 m_EnabledProcessors;
        std::vector<Box<IProcessor>>                 m_DisabledProcessors;

        bool makeProcessor(TypeID id, Box<IProcessor>&& processor);
        bool destroyProcessor(TypeID id);
        bool toggleProcessor(TypeID id, bool enable);
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::world::World);

/* ===== Inline API Implementations ===== */
export namespace ryn::world {
    Entity EntityAPI::create() { return World.m_Registry.create(); }

    void EntityAPI::destroy(Entity entity) { World.m_Registry.destroy(entity); }

    template<typename T, typename... Args>
    requires IsComponent<T>
    T& ComponentAPI::make(Entity entity, Args&&... args) {
        using EnTTResult = decltype(World.m_Registry.emplace<T>(entity, std::forward<Args>(args)...));

        if constexpr (std::is_void_v<EnTTResult>) {
            World.m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
            static T dummy{};
            return dummy;
        } else {
            return World.m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
        }
    }

    template<typename T, typename... Args>
    requires IsComponent<T>
    T& ComponentAPI::getOrMake(Entity entity, Args&&... args) {
        using EnTTResult = decltype(World.m_Registry.get_or_emplace<T>(entity, std::forward<Args>(args)...));

        if constexpr (std::is_void_v<EnTTResult>) {
            World.m_Registry.get_or_emplace<T>(entity, std::forward<Args>(args)...);
            static T dummy{};
            return dummy;
        } else {
            return World.m_Registry.get_or_emplace<T>(entity, std::forward<Args>(args)...);
        }
    }

    template<typename T>
    requires IsComponent<T>
    T& ComponentAPI::get(Entity entity) {
        using EnTTResult = decltype(World.m_Registry.get<T>(entity));

        if constexpr (std::is_void_v<EnTTResult>) {
            RYN_ASSERT(World.m_Registry.all_of<T>(entity), "Entity does not have tag component!");
            static T dummy{};
            return dummy;
        } else {
            return World.m_Registry.get<T>(entity);
        }
    }

    template<typename T>
    requires IsComponent<T>
    T* ComponentAPI::tryGet(Entity entity) {
        return World.m_Registry.try_get<T>(entity);
    }

    template<typename T>
    requires IsComponent<T>
    bool ComponentAPI::has(Entity entity) const {
        return World.m_Registry.all_of<T>(entity);
    }

    template<typename T>
    requires IsComponent<T>
    bool ComponentAPI::destroy(Entity entity) {
        return World.m_Registry.remove<T>(entity) > 0;
    }

    template<typename T, typename... Args>
    requires IsProcessor<T>
    T* ProcessorAPI::make(Args&&... args) {
        Box<IProcessor> processorBox = Box<T>::template make<T>(std::forward<Args>(args)...);
        IProcessor*     processor    = processorBox.get();
        return World.makeProcessor(T::TYPE_ID, std::move(processorBox)) ? static_cast<T*>(processor) : nullptr;
    }

    template<typename T>
    requires IsProcessor<T>
    bool ProcessorAPI::toggle(bool enable) {
        return World.toggleProcessor(T::TYPE_ID, enable);
    }

    template<typename T>
    requires IsProcessor<T>
    bool ProcessorAPI::destroy() {
        return World.destroyProcessor(T::TYPE_ID);
    }

    template<typename T, typename... Args>
    requires IsComponent<T>
    T& GlobalAPI::make(Args&&... args) {
        return World.m_Registry.ctx().emplace<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    requires IsComponent<T>
    T& GlobalAPI::getOrMake(Args&&... args) {
        if (T* ptr = World.m_Registry.ctx().find<T>()) return *ptr;
        return World.m_Registry.ctx().emplace<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    requires IsComponent<T>
    T& GlobalAPI::get() {
        return World.m_Registry.ctx().get<T>();
    }

    template<typename T>
    requires IsComponent<T>
    bool GlobalAPI::has() {
        return World.m_Registry.ctx().contains<T>();
    }

    template<typename T>
    requires IsComponent<T>
    bool GlobalAPI::destroy() {
        return World.m_Registry.ctx().erase<T>() > 0;
    }

    template<typename T, auto Candidate>
    requires IsComponent<T>
    void ObserverAPI::onComponentMake() {
        World.m_Registry.on_construct<T>().template connect<Candidate>();
    }

    template<typename T, auto Candidate>
    requires IsComponent<T>
    void ObserverAPI::onComponentDestroy() {
        World.m_Registry.on_destroy<T>().template connect<Candidate>();
    }

    template<typename T, auto Candidate>
    requires IsComponent<T>
    void ObserverAPI::offComponentMake() {
        World.m_Registry.on_construct<T>().template disconnect<Candidate>();
    }

    template<typename T, auto Candidate>
    requires IsComponent<T>
    void ObserverAPI::offComponentDestroy() {
        World.m_Registry.on_destroy<T>().template disconnect<Candidate>();
    }

    template<typename T, auto Candidate, typename Type>
    requires IsComponent<T>
    ObserverConnection ObserverAPI::onComponentMake(Type* inst) {
        return ObserverConnection{World.m_Registry.on_construct<T>().template connect<Candidate>(inst)};
    }

    template<typename T, auto Candidate, typename Type>
    requires IsComponent<T>
    ObserverConnection ObserverAPI::onComponentDestroy(Type* inst) {
        return ObserverConnection{World.m_Registry.on_destroy<T>().template connect<Candidate>(inst)};
    }

    template<typename T, auto Candidate, typename Type>
    requires IsComponent<T>
    void ObserverAPI::offComponentMake(Type* inst) {
        World.m_Registry.on_construct<T>().template disconnect<Candidate>(inst);
    }

    template<typename T, auto Candidate, typename Type>
    requires IsComponent<T>
    void ObserverAPI::offComponentDestroy(Type* inst) {
        World.m_Registry.on_destroy<T>().template disconnect<Candidate>(inst);
    }

    template<typename T>
    requires IsComponent<T>
    Span<T> ViewAPI::all() {
        auto& storage = World.m_Registry.storage<T>();
        return Span<T>(storage.data(), storage.size());
    }

    template<IsInclusionTag Inclusion, IsExclusionTag Exclusion, typename Func>
    void ViewAPI::foreach (Func&& func) {
        foreachImplementation(Inclusion{}, Exclusion{}, std::forward<Func>(func));
    }

    template<typename... I, typename... E, typename Func>
    void ViewAPI::foreachImplementation(Incl<I...>, Excl<E...>, Func&& func) {
        auto view = World.m_Registry.view<I...>(entt::exclude<E...>);

        view.each([&func](entt::entity e, I&... components) { func(Entity(e), components...); });
    }
}