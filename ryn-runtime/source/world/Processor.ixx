module;
#include "Prelude.h"
#include <concepts>

export module Ryntime.World:Processor;
export import Ryntime.Core;

using namespace ryn::reflection;

export namespace ryn::world {
    class World;

    class RYN_API ServiceDiscovery : public IServiceLocator {
    public:
        explicit ServiceDiscovery(IServiceLocator& locator);
        [[nodiscard]] inline bool wasSuccessful() const noexcept { return m_Success; }

    protected:
        IServiceLocator& m_Locator;
        bool             m_Success = true;

        IService* requestService(TypeID typeId, bool required) override;
        void      notifyMissingService(TypeID typeId) override;
    };

    class RYN_API IProcessor : public DynamicBase<TypeCategory::Processor> {
    public:
        ~IProcessor() override;

        virtual inline void onConfigure([[maybe_unused]] ServiceDiscovery& discovery) {}

        virtual inline void onEnable() {}
        virtual inline void onDisable() {}

        virtual inline void onBeginFrame([[maybe_unused]] const FrameTime time) {}
        virtual inline void onUpdate([[maybe_unused]] const FrameTime time) {}
        virtual inline void onEndFrame([[maybe_unused]] const FrameTime time) {}

    protected:
        friend World;
        World* m_World = nullptr;
    };

    template<typename T>
    concept IsProcessor = std::derived_from<T, IProcessor> && IsType<T>;
}