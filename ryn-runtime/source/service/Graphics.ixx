module;
#include "Prelude.h"
#include <SwapChain.h>
#include <cstdint>

export module Ryntime.Service:Graphics;
export import Ryntime.Core;
export import Ryntime.Core.Reflection;

using namespace ryn::reflection;

export namespace ryn::service {
    enum class GraphicsBackend : uint8_t {
        Unknown   = 0,
        Vulkan    = 1,
        DirectX11 = 2,
        DirectX12 = 3,
    };

    enum class GraphicsVSync : uint8_t {
        Disable     = 0,
        Enable      = 1,
        HalfRefresh = 2,
    };

    struct GraphicsConfig {
        GraphicsBackend Backend = GraphicsBackend::Vulkan;
        GraphicsVSync   VSync   = GraphicsVSync::Enable;
    };

    class RYN_API Graphics final : public DynamicType<IService, "ryn::service::Graphics"> {
    public:
        explicit Graphics(const GraphicsConfig& config);
        ~Graphics() override;

        void onAttach(IServiceLocator& locator) override;
        void onDetach() override;
        void onBeginFrame(const FrameTime& time) override;
        void onEndFrame(const FrameTime& time) override;
        void onEvent(EventListener& listener) override;

        [[nodiscard]] graphics::IDevice&    getDevice() const;
        [[nodiscard]] graphics::IContext&   getContext() const;
        [[nodiscard]] graphics::ISwapchain& getSwapchain() const;

    private:
        const GraphicsBackend m_CurrentBackend = GraphicsBackend::Unknown;
        GraphicsVSync         m_CurrentVSync   = GraphicsVSync::Disable;

        graphics::SharedRef<graphics::IDevice>    m_Device;
        graphics::SharedRef<graphics::IContext>   m_Context;
        graphics::SharedRef<graphics::ISwapchain> m_Swapchain;

        void initVulkan(const Diligent::NativeWindow& nativeWindow, const Diligent::SwapChainDesc& swapchainDesc);
#if defined(RYN_PLATFORM_WIN)
        void initD3D12(const Diligent::NativeWindow& nativeWindow, const Diligent::SwapChainDesc& swapchainDesc);
        void initD3D11(const Diligent::NativeWindow& nativeWindow, const Diligent::SwapChainDesc& swapchainDesc);
#endif
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::service::Graphics);