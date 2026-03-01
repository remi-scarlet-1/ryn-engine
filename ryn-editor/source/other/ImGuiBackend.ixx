module;
#include "Prelude.h"
#include <cstdint>

export module Ryndition.ImGuiBackend;
export import Ryntime;

using namespace ryn::graphics;
using namespace ryn;

export namespace rynd::other {
    class ImGuiBackend {
    public:
        explicit ImGuiBackend(const service::Graphics& graphicsService);
        ~ImGuiBackend();

        void newFrame();
        void render();
        void createDeviceObjects();
        void invalidateDeviceObjects();

    private:
        static constexpr uint32_t INIT_VB_SIZE = 4096;
        static constexpr uint32_t INIT_IB_SIZE = 8192;

        IDevice*    m_Device    = nullptr;
        IContext*   m_Context   = nullptr;
        ISwapchain* m_Swapchain = nullptr;

        SharedRef<IBuffer>                m_VB;
        SharedRef<IBuffer>                m_VBConstants;
        SharedRef<IBuffer>                m_IB;
        SharedRef<IPipelineState>         m_PSO;
        SharedRef<IShaderResourceBinding> m_SRB;
        IShaderResourceVariable*          m_TextureVariable = nullptr;

        bool             m_BaseVertexSupport   = false;
        uint32_t         m_VBSize              = INIT_VB_SIZE;
        uint32_t         m_IBSize              = INIT_IB_SIZE;
        Vector2Int       m_SurfaceRes          = Vector2Int::zero();
        SurfaceTransform m_SurfacePreTransform = SurfaceTransforms::IDENTITY;
    };
}