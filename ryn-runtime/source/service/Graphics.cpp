module;
#include "Prelude.h"
#include <DeviceContext.h>
#include <RefCntAutoPtr.hpp>
#include <RenderDevice.h>
#include <SwapChain.h>

#include <EngineFactoryVk.h>
#if defined(RYN_PLATFORM_WIN)
    #include <EngineFactoryD3D11.h>
    #include <EngineFactoryD3D12.h>
#endif

module Ryntime.Service;

RYN_SCOPE_FILE("Service|Graphics");

namespace {
    Diligent::NativeWindow getNativeWindow(const ryn::service::WindowHandles& handles);
    void                   debugMessageCallback(Diligent::DEBUG_MESSAGE_SEVERITY severity,
                                                const Diligent::Char*            message,
                                                const Diligent::Char*            function,
                                                const Diligent::Char*            file,
                                                int                              line);
}

namespace ryn::service {
    Graphics::Graphics(const GraphicsConfig& config) : m_CurrentBackend(config.Backend), m_CurrentVSync(config.VSync) {}
    Graphics::~Graphics() = default;

    void Graphics::onAttach(IServiceLocator& locator) {
        const auto* windowService   = locator.require<Window>();
        const auto  framebufferSize = windowService->getFramebufferSize();

        // Setup Diligent debug callback
        Diligent::SetDebugMessageCallback(debugMessageCallback);

        // Describe swapchain
        const Diligent::NativeWindow nativeWindow = getNativeWindow(windowService->getHandles());

        Diligent::SwapChainDesc swapchainDesc;
        swapchainDesc.Usage             = Diligent::SWAP_CHAIN_USAGE_RENDER_TARGET;
        swapchainDesc.ColorBufferFormat = Diligent::TEX_FORMAT_RGBA8_UNORM;
        swapchainDesc.DepthBufferFormat = Diligent::TEX_FORMAT_UNKNOWN;
        swapchainDesc.Width             = framebufferSize.x;
        swapchainDesc.Height            = framebufferSize.y;

        // Initialize backend
        switch (m_CurrentBackend) {
#if defined(RYN_PLATFORM_LINUX) || defined(RYN_PLATFORM_WIN)
            case GraphicsBackend::Vulkan: initVulkan(nativeWindow, swapchainDesc); break;
#endif
#if defined(RYN_PLATFORM_WIN)
            case GraphicsBackend::DirectX12: initD3D12(nativeWindow, swapchainDesc); break;

            case GraphicsBackend::DirectX11: initD3D11(nativeWindow, swapchainDesc); break;
#endif

            default: RYN_CRITICAL("Selected Graphics Backend is not supported on this platform!");
        }

        RYN_VERIFY(m_Device && m_Context, "Failed to initialize Render Device!");
        RYN_VERIFY(m_Swapchain, "Failed to create SwapChain!");
        RYN_INFO("Successfully initialized Diligent Graphics");
    }

    void Graphics::onDetach() {
        RYN_TRACE("Shutdown Diligent Graphics...");
        if (m_Context) m_Context->Flush();

        m_Swapchain.Release();
        m_Context.Release();
        m_Device.Release();
    }

    void Graphics::onBeginFrame([[maybe_unused]] const FrameTime& time) {
        static constexpr float CLEAR_COLOR[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        auto*                  colorView      = m_Swapchain->GetCurrentBackBufferRTV();

        m_Context->SetRenderTargets(1, &colorView, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_Context->ClearRenderTarget(colorView, CLEAR_COLOR, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    void Graphics::onEndFrame([[maybe_unused]] const FrameTime& time) { m_Swapchain->Present(static_cast<uint32_t>(m_CurrentVSync)); }

    void Graphics::onEvent(EventListener& listener) {
        listener.match<WindowFramebufferResizeEvent>([this](WindowFramebufferResizeEvent& e) {
            if (e.Resolution.x > 0 && e.Resolution.y > 0) {
                RYN_TRACE("Resizing swapchain to {0}x{1}", e.Resolution.x, e.Resolution.y);
                m_Swapchain->Resize(e.Resolution.x, e.Resolution.y);
                e.consume();
            }
        });
    }

    graphics::IDevice&    Graphics::getDevice() const { return *m_Device; }
    graphics::IContext&   Graphics::getContext() const { return *m_Context; }
    graphics::ISwapchain& Graphics::getSwapchain() const { return *m_Swapchain; }

    void Graphics::initVulkan(const Diligent::NativeWindow& nativeWindow, const Diligent::SwapChainDesc& swapchainDesc) {
        RYN_TRACE("Initialize Diligent Graphics 'Vulkan'...");
        const auto factory = Diligent::GetEngineFactoryVk();
        RYN_VERIFY(factory, "Failed to load Vulkan factory!");

        Diligent::EngineVkCreateInfo info;
#if defined(RYN_CONFIG_DEBUG)
        info.EnableValidation = true;
#endif

        factory->CreateDeviceAndContextsVk(info, &m_Device, &m_Context);
        if (m_Device && m_Context) factory->CreateSwapChainVk(m_Device, m_Context, swapchainDesc, nativeWindow, &m_Swapchain);
    }

#if defined(RYN_PLATFORM_WIN)
    void Graphics::initD3D12(const Diligent::NativeWindow& nativeWindow, const Diligent::SwapChainDesc& swapchainDesc) {
        RYN_TRACE("Initialize Diligent Graphics 'Direct3D 12'...");
        const auto factory = Diligent::GetEngineFactoryD3D12();
        RYN_VERIFY(factory, "Failed to load D3D12 factory!");

        Diligent::EngineD3D12CreateInfo info;
    #if defined(RYN_CONFIG_DEBUG)
        info.EnableValidation = true;
    #endif

        factory->CreateDeviceAndContextsD3D12(info, &m_Device, &m_Context);
        if (m_Device && m_Context) factory->CreateSwapChainD3D12(m_Device, m_Context, swapchainDesc, {}, nativeWindow, &m_Swapchain);
    }

    void Graphics::initD3D11(const Diligent::NativeWindow& nativeWindow, const Diligent::SwapChainDesc& swapchainDesc) {
        RYN_TRACE("Initialize Diligent Graphics 'Direct3D 11'...");
        const auto factory = Diligent::GetEngineFactoryD3D11();
        RYN_VERIFY(factory, "Failed to load D3D11 factory!");

        Diligent::EngineD3D11CreateInfo info;
    #if defined(RYN_CONFIG_DEBUG)
        info.EnableValidation = true;
    #endif

        factory->CreateDeviceAndContextsD3D11(info, &m_Device, &m_Context);
        if (m_Device && m_Context) factory->CreateSwapChainD3D11(m_Device, m_Context, swapchainDesc, {}, nativeWindow, &m_Swapchain);
    }
#endif
}

namespace {
    Diligent::NativeWindow getNativeWindow(const ryn::service::WindowHandles& handles) {
        Diligent::NativeWindow nativeWindow;

#if defined(RYN_PLATFORM_LINUX)
        switch (handles.NativeType) {
            case ryn::service::WindowNativeType::Wayland:
                nativeWindow.pWaylandSurface = handles.NativeHandle;
                nativeWindow.pDisplay        = handles.NativeDisplayHandle;
                return nativeWindow;

            case ryn::service::WindowNativeType::X11:
                // X11 expects a uint32_t Window ID, but we get a void* pointer.
                // We cast via uintptr_t to safely truncate on 64-bit systems without warnings.
                nativeWindow.WindowId = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handles.NativeHandle));
                nativeWindow.pDisplay = handles.NativeDisplayHandle;
                return nativeWindow;

            default: RYN_ERROR("Unknown Linux window type!"); return nativeWindow;
        }
#elif defined(RYN_PLATFORM_WIN)
        if (handles.NativeType == ryn::service::WindowNativeType::Win32) {
            nativeWindow.hWnd = handles.NativeHandle;
            return nativeWindow;
        }

        RYN_ERROR("Expected win32 handle but was given other type!");
        return nativeWindow;
#else
    #error "Unsupported platform for graphics"
#endif
    }

    void debugMessageCallback(const Diligent::DEBUG_MESSAGE_SEVERITY severity,
                              const Diligent::Char*                  message,
                              const Diligent::Char*                  function,
                              const Diligent::Char*                  file,
                              int                                    line) {
        RYN_SCOPE_LOCAL("Service|Graphics|Diligent");
        switch (severity) {
            case Diligent::DEBUG_MESSAGE_SEVERITY_INFO: RYN_TRACE("{0}", message); break;
            case Diligent::DEBUG_MESSAGE_SEVERITY_WARNING: RYN_WARN("{0}", message); break;
            case Diligent::DEBUG_MESSAGE_SEVERITY_ERROR: RYN_ERROR("{0} ({1} at {2}:{3})", message, function, file, line); break;
            case Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR: RYN_CRITICAL("{0}", message); break;
        }
    }
}