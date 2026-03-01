module;
#include "Prelude.h"
#include <cstdint>
#include <imgui.h>
#include <vector>

module Ryndition.ImGuiBackend;

namespace {
    constexpr auto HLSL_VERTEX_SHADER = R"(
            cbuffer Constants {
                float4x4 ProjectionMatrix;
            }

            struct VSInput {
                float2 pos : ATTRIB0;
                float2 uv  : ATTRIB1;
                float4 col : ATTRIB2;
            };

            struct PSInput {
                float4 pos : SV_POSITION;
                float4 col : COLOR;
                float2 uv  : TEXCOORD;
            };

            void main(in VSInput VSIn, out PSInput PSIn) {
                PSIn.pos = mul(ProjectionMatrix, float4(VSIn.pos.xy, 0.0, 1.0));
                PSIn.col = VSIn.col;
                PSIn.uv  = VSIn.uv;
            }
        )";

    constexpr auto HLSL_PIXEL_SHADER = R"(
            struct PSInput {
                float4 pos : SV_POSITION;
                float4 col : COLOR;
                float2 uv  : TEXCOORD;
            };

            Texture2D    Texture;
            SamplerState Texture_sampler;

            float4 main(in PSInput PSIn) : SV_Target {
                float4 col = Texture.Sample(Texture_sampler, PSIn.uv) * PSIn.col;
                col.rgb *= col.a;
                SRGBA_TO_LINEAR(col)
                return col;
            }
        )";

    constexpr auto HLSL_MACRO_GAMMA_TO_LINEAR = "((Gamma) < 0.04045 ? (Gamma) / 12.92 : pow(max((Gamma) + 0.055, 0.0) / 1.055, 2.4))";

    constexpr auto HLSL_MACRO_SRGB_TO_LINEAR = R"(
            col.r = GAMMA_TO_LINEAR(col.r);
            col.g = GAMMA_TO_LINEAR(col.g);
            col.b = GAMMA_TO_LINEAR(col.b);
            col.a = 1.0 - GAMMA_TO_LINEAR(1.0 - col.a);
        )";

    TextureFormat convertTextureFormat(ImTextureFormat imFormat);
    Vector4       transformScissor(const ImVec2& displaySize, const Vector4& rect, SurfaceTransform surfaceTransform);
    void          updateTexture(IDevice* device, IContext* context, ImTextureData* imTextureData);
    void          destroyTexture(ImTextureData* imTextureData);
}

namespace rynd::other {
    ImGuiBackend::ImGuiBackend(const service::Graphics& graphicsService) {
        // Retrieve devices (assume caller ensures the validity of the graphics layer during lifetime)
        m_Device    = &graphicsService.getDevice();
        m_Context   = &graphicsService.getContext();
        m_Swapchain = &graphicsService.getSwapchain();

        // Check base vertex support
        m_BaseVertexSupport = m_Device->GetAdapterInfo().DrawCommand.CapFlags & DrawCommandCapFlags::BASE_VERTEX;

        // Setup ImGui backend settings
        ImGuiIO& io            = ImGui::GetIO();
        io.BackendRendererName = "ryn_editor::details::ImGuiBackend";
        io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;
        io.BackendFlags |= m_BaseVertexSupport ? ImGuiBackendFlags_RendererHasVtxOffset : 0;

        createDeviceObjects();
    }

    ImGuiBackend::~ImGuiBackend() { invalidateDeviceObjects(); }

    void ImGuiBackend::newFrame() {
        // Ensure graphics resources are valid
        if (!m_PSO) createDeviceObjects();

        // Update runtime state
        const auto& swapchainDesc = m_Swapchain->GetDesc();
        m_SurfaceRes              = Vector2Int(swapchainDesc.Width, swapchainDesc.Height);
        m_SurfacePreTransform     = swapchainDesc.PreTransform;
    }

    void ImGuiBackend::render() {
        ImDrawData* imDrawData = ImGui::GetDrawData();

        // Catch up with texture updates
        // Most of the times this list will have 1 element with an OK status (nothing to do)
        if (imDrawData->Textures != nullptr)
            for (ImTextureData* imTextureData : *imDrawData->Textures)
                if (imTextureData->Status != ImTextureStatus_OK) updateTexture(m_Device, m_Context, imTextureData);

        // Skip rendering if minimized
        if (imDrawData->DisplaySize.x <= 0.0f || imDrawData->DisplaySize.y <= 0.0f || imDrawData->CmdLists.empty()) return;

        // Create and grow vertex/index buffers if needed
        if (!m_VB || static_cast<int>(m_VBSize) < imDrawData->TotalVtxCount) {
            m_VB.Release();

            if (m_VBSize == 0) m_VBSize = INIT_VB_SIZE;
            while (static_cast<int>(m_VBSize) < imDrawData->TotalVtxCount)
                m_VBSize *= 2;

            BufferDesc vertexBufferDecs;
            vertexBufferDecs.Name           = "ImGui Vertex Buffer";
            vertexBufferDecs.Size           = m_VBSize * sizeof(ImDrawVert);
            vertexBufferDecs.BindFlags      = BindFlags::VERTEX_BUFFER;
            vertexBufferDecs.Usage          = Usages::DYNAMIC;
            vertexBufferDecs.CPUAccessFlags = CpuAccessFlags::WRITE;
            m_Device->CreateBuffer(vertexBufferDecs, nullptr, &m_VB);
        }

        if (!m_IB || static_cast<int>(m_IBSize) < imDrawData->TotalIdxCount) {
            m_IB.Release();

            if (m_IBSize == 0) m_IBSize = INIT_IB_SIZE;
            while (static_cast<int>(m_IBSize) < imDrawData->TotalIdxCount)
                m_IBSize *= 2;

            BufferDesc indexBufferDesc;
            indexBufferDesc.Name           = "ImGui Index Buffer";
            indexBufferDesc.Size           = m_IBSize * sizeof(ImDrawIdx);
            indexBufferDesc.BindFlags      = BindFlags::INDEX_BUFFER;
            indexBufferDesc.Usage          = Usages::DYNAMIC;
            indexBufferDesc.CPUAccessFlags = CpuAccessFlags::WRITE;
            m_Device->CreateBuffer(indexBufferDesc, nullptr, &m_IB);
        }

        {
            MapHelper<ImDrawVert> vertices{m_Context, m_VB, MapTypes::WRITE, MapFlags::DISCARD};
            MapHelper<ImDrawIdx>  indices{m_Context, m_IB, MapTypes::WRITE, MapFlags::DISCARD};
            if (!vertices || !indices) return;

            ImDrawVert* vertexDestination = vertices;
            ImDrawIdx*  indexDestination  = indices;
            for (const ImDrawList* cmdList : imDrawData->CmdLists) {
                memcpy(vertexDestination, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
                memcpy(indexDestination, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
                vertexDestination += cmdList->VtxBuffer.Size;
                indexDestination += cmdList->IdxBuffer.Size;
            }
        }

        // Setup orthographic projection matrix and upload into vertex shader constant buffer
        // Our visible imgui space lies from imDrawData->DisplayPos (top left) to imDrawData->DisplayPos + imDrawData->DisplaySize (bottom right).
        {
            // DisplaySize always refers to the logical dimensions that account for pre-transform,
            // hence the aspect ratio will be correct after applying appropriate rotation.
            float L = imDrawData->DisplayPos.x;
            float R = imDrawData->DisplayPos.x + imDrawData->DisplaySize.x;
            float T = imDrawData->DisplayPos.y;
            float B = imDrawData->DisplayPos.y + imDrawData->DisplaySize.y;

            // clang-format off
            Matrix4x4 projectionMatrix {
                2.0f / (R - L),     0.0f,               0.0f,   0.0f,
                0.0f,               2.0f / (T - B),     0.0f,   0.0f,
                0.0f,               0.0f,               0.5f,   0.0f,
                (R + L) / (L - R),  (T + B) / (B - T),  0.5f,   1.0f
            };
            // clang-format on

            // Bake pre-transform into projection (rotations are clockwise)
            switch (m_SurfacePreTransform) {
            case SurfaceTransforms::IDENTITY:
                break;

            case SurfaceTransforms::ROTATE_90:
                projectionMatrix *= Quaternion::fromAngleAxis(-90.0f, Vector3::forward());
                break;

            case SurfaceTransforms::ROTATE_180:
                projectionMatrix *= Quaternion::fromAngleAxis(-180.0f, Vector3::forward());
                break;

            case SurfaceTransforms::ROTATE_270:
                projectionMatrix *= Quaternion::fromAngleAxis(-270.0f, Vector3::forward());
                break;

            case SurfaceTransforms::OPTIMAL:
                RYN_VERIFY(false, "SURFACE_TRANSFORM_OPTIMAL is only valid as parameter during swapchain initialization!");
                break;

            case SurfaceTransforms::HORIZONTAL_MIRROR:
            case SurfaceTransforms::HORIZONTAL_MIRROR_ROTATE_90:
            case SurfaceTransforms::HORIZONTAL_MIRROR_ROTATE_180:
            case SurfaceTransforms::HORIZONTAL_MIRROR_ROTATE_270:
                RYN_VERIFY(false, "Mirror surface tranforms are not supported!");
                break;

            default:
                RYN_VERIFY(false, "Unknown surface transform!");
            }

            MapHelper<Matrix4x4> constantBuffer{m_Context, m_VBConstants, MapTypes::WRITE, MapFlags::DISCARD};
            if (!constantBuffer) return;
            *constantBuffer = projectionMatrix;
        }

        auto setupRenderState = [&] {
            // Setup shader and vertex buffers
            IBuffer* vertexBuffers[] = {m_VB};
            m_Context->SetVertexBuffers(0, 1, vertexBuffers, nullptr, ResourceStateTransitionModes::TRANSITION, SetVertexBuffersFlags::RESET);
            m_Context->SetIndexBuffer(m_IB, 0, ResourceStateTransitionModes::TRANSITION);
            m_Context->SetPipelineState(m_PSO);

            constexpr float blendFactor[4] = {0.f, 0.f, 0.f, 0.f};
            m_Context->SetBlendFactors(blendFactor);

            Viewport viewport;
            viewport.TopLeftX = 0;
            viewport.TopLeftY = 0;
            viewport.Width    = m_SurfaceRes.x;
            viewport.Height   = m_SurfaceRes.y;
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            m_Context->SetViewports(1, &viewport, m_SurfaceRes.x, m_SurfaceRes.y);
        };

        setupRenderState();

        // Render command lists
        // (Because we merged all buffers into a single one, we maintain our own offset into them)
        uint32_t globalIBOffset = 0;
        uint32_t globalVBOffset = 0;

        ITextureView* lastTextureView = nullptr;
        for (const ImDrawList* cmdList : imDrawData->CmdLists) {
            for (const ImDrawCmd& cmdRef : cmdList->CmdBuffer) {
                const ImDrawCmd* cmd = &cmdRef;

                if (cmd->UserCallback != nullptr) {
                    // User callback, registered via ImDrawList::AddCallback()
                    // (ImDrawCallback_ResetRenderState request to reset render state because user messed it up)
                    if (cmd->UserCallback == ImDrawCallback_ResetRenderState) setupRenderState();
                    else cmd->UserCallback(cmdList, cmd);
                } else {
                    if (cmd->ElemCount == 0) continue;

                    // Apply scissor
                    Vector4 scissorRelative{
                        (cmd->ClipRect.x - imDrawData->DisplayPos.x) * imDrawData->FramebufferScale.x, // x
                        (cmd->ClipRect.y - imDrawData->DisplayPos.y) * imDrawData->FramebufferScale.y, // y
                        (cmd->ClipRect.z - imDrawData->DisplayPos.x) * imDrawData->FramebufferScale.x, // width
                        (cmd->ClipRect.w - imDrawData->DisplayPos.y) * imDrawData->FramebufferScale.y  // height
                    };

                    scissorRelative = transformScissor(imDrawData->DisplaySize, scissorRelative, m_SurfacePreTransform);
                    Rect scissorTransformed{static_cast<int32_t>(scissorRelative.x),
                                            static_cast<int32_t>(scissorRelative.y),
                                            static_cast<int32_t>(scissorRelative.z),
                                            static_cast<int32_t>(scissorRelative.w)};

                    scissorTransformed.left   = std::max(scissorTransformed.left, 0);
                    scissorTransformed.top    = std::max(scissorTransformed.top, 0);
                    scissorTransformed.right  = std::min(scissorTransformed.right, static_cast<int32_t>(m_SurfaceRes.x));
                    scissorTransformed.bottom = std::min(scissorTransformed.bottom, static_cast<int32_t>(m_SurfaceRes.y));

                    if (!scissorTransformed.IsValid()) continue;

                    m_Context->SetScissorRects(1, &scissorTransformed, m_SurfaceRes.x, m_SurfaceRes.y);

                    // Bind texture
                    auto* textureView = reinterpret_cast<ITextureView*>(cmd->GetTexID());
                    RYN_VERIFY(textureView != nullptr, "TextureView shouldn't be null!");

                    // If required, update shader resource (texture)
                    if (textureView != lastTextureView) {
                        lastTextureView = textureView;
                        m_TextureVariable->Set(textureView);
                        m_Context->CommitShaderResources(m_SRB, ResourceStateTransitionModes::TRANSITION);
                    }

                    // Setup draw attributes for current ImGui element and execute draw
                    DrawIndexedAttribs drawAttribs{cmd->ElemCount,
                                                   sizeof(ImDrawIdx) == sizeof(uint16_t) ? ValueTypes::VT_UINT16 : ValueTypes::VT_UINT32,
                                                   DrawFlags::VERIFY_STATES};
                    drawAttribs.FirstIndexLocation = cmd->IdxOffset + globalIBOffset;
                    if (m_BaseVertexSupport) {
                        drawAttribs.BaseVertex = cmd->VtxOffset + globalVBOffset;
                    } else {
                        IBuffer* VBs[]       = {m_VB};
                        uint64_t VBOffsets[] = {sizeof(ImDrawVert) * (size_t{cmd->VtxOffset} + size_t{globalVBOffset})};
                        m_Context->SetVertexBuffers(0, 1, VBs, VBOffsets, ResourceStateTransitionModes::TRANSITION, SetVertexBuffersFlags::NONE);
                    }
                    m_Context->DrawIndexed(drawAttribs);
                }
            }

            globalIBOffset += cmdList->IdxBuffer.Size;
            globalVBOffset += cmdList->VtxBuffer.Size;
        }
    }

    void ImGuiBackend::createDeviceObjects() {
        invalidateDeviceObjects();

        const auto fmtBackBuffer  = m_Swapchain->GetDesc().ColorBufferFormat;
        const auto fmtDepthBuffer = m_Swapchain->GetDesc().DepthBufferFormat;

        const auto fmtAttribs = getTextureFormatAttribs(fmtBackBuffer);
        const bool isSRGB     = fmtAttribs.ComponentType == ComponentTypes::UNORM_SRGB;

        // Initialize shader macros
        std::vector<ShaderMacro> macros;
        if (isSRGB) {
            macros.emplace_back("GAMMA_TO_LINEAR(Gamma)", HLSL_MACRO_GAMMA_TO_LINEAR);
            macros.emplace_back("SRGBA_TO_LINEAR(col)", HLSL_MACRO_SRGB_TO_LINEAR);
        } else macros.emplace_back("SRGBA_TO_LINEAR(col)", "");

        // Create vertex & pixel shaders
        ShaderCreateInfo shaderCI;
        shaderCI.SourceLanguage = ShaderSourceLanguages::HLSL;
        shaderCI.Macros         = {macros.data(), static_cast<uint32_t>(macros.size())};

        SharedRef<IShader> vertexShader;
        {
            shaderCI.Desc   = {"ImGui Vertex Shader", ShaderTypes::VERTEX, true};
            shaderCI.Source = HLSL_VERTEX_SHADER;
            m_Device->CreateShader(shaderCI, &vertexShader);
        }

        SharedRef<IShader> pixelShader;
        {
            shaderCI.Desc   = {"ImGui Pixel Shader", ShaderTypes::PIXEL, true};
            shaderCI.Source = HLSL_PIXEL_SHADER;
            m_Device->CreateShader(shaderCI, &pixelShader);
        }

        GraphicsPipelineStateCreateInfo psoCI;
        psoCI.PSODesc.Name = "ImGui PSO";

        // Configure PSO shaders
        psoCI.pVS = vertexShader;
        psoCI.pPS = pixelShader;

        // Configure PSO graphics pipeline
        GraphicsPipelineDesc& psoCI_GraphicsPipeline{psoCI.GraphicsPipeline};
        psoCI_GraphicsPipeline.NumRenderTargets             = 1;
        psoCI_GraphicsPipeline.RTVFormats[0]                = fmtBackBuffer;
        psoCI_GraphicsPipeline.DSVFormat                    = fmtDepthBuffer;
        psoCI_GraphicsPipeline.PrimitiveTopology            = PrimitiveTopologys::TRIANGLE_LIST;
        psoCI_GraphicsPipeline.RasterizerDesc.CullMode      = CullModes::NONE;
        psoCI_GraphicsPipeline.RasterizerDesc.ScissorEnable = true;
        psoCI_GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

        // Configure PSO main render target blending (alpha-premultiplied)
        RenderTargetBlendDesc& psoCI_RTBlend{psoCI.GraphicsPipeline.BlendDesc.RenderTargets[0]};
        psoCI_RTBlend.BlendEnable           = true;
        psoCI_RTBlend.SrcBlend              = BlendFactors::ONE;
        psoCI_RTBlend.DestBlend             = BlendFactors::INV_SRC_ALPHA;
        psoCI_RTBlend.BlendOp               = BlendOperations::ADD;
        psoCI_RTBlend.SrcBlendAlpha         = BlendFactors::ONE;
        psoCI_RTBlend.DestBlendAlpha        = BlendFactors::INV_SRC_ALPHA;
        psoCI_RTBlend.BlendOpAlpha          = BlendOperations::ADD;
        psoCI_RTBlend.RenderTargetWriteMask = ColorMasks::ALL;

        // Configure PSO vertex shader inputs
        LayoutElement vertexShaderInputs[] = {
            {0, 0, 2, ValueTypes::VT_FLOAT32},    // pos
            {1, 0, 2, ValueTypes::VT_FLOAT32},    // uv
            {2, 0, 4, ValueTypes::VT_UINT8, true} // col
        };

        psoCI_GraphicsPipeline.InputLayout.LayoutElements = vertexShaderInputs;
        psoCI_GraphicsPipeline.InputLayout.NumElements    = std::size(vertexShaderInputs);

        // Configure PSO shader resource variables
        ShaderResourceVariableDesc shaderResourceVariables[] = {{ShaderTypes::PIXEL, "Texture", ShaderResourceVariableTypes::DYNAMIC}};

        psoCI.PSODesc.ResourceLayout.Variables    = shaderResourceVariables;
        psoCI.PSODesc.ResourceLayout.NumVariables = std::size(shaderResourceVariables);

        // Configure PSO texture samplers
        SamplerDesc samplerLinearWrap;
        samplerLinearWrap.AddressU              = TextureAddressModes::WRAP;
        samplerLinearWrap.AddressV              = TextureAddressModes::WRAP;
        samplerLinearWrap.AddressW              = TextureAddressModes::WRAP;
        ImmutableSamplerDesc immutableSamples[] = {{ShaderTypes::PIXEL, "Texture", samplerLinearWrap}};

        psoCI.PSODesc.ResourceLayout.ImmutableSamplers    = immutableSamples;
        psoCI.PSODesc.ResourceLayout.NumImmutableSamplers = std::size(immutableSamples);

        m_Device->CreateGraphicsPipelineState(psoCI, &m_PSO);

        // Configure PSO constants
        {
            BufferDesc bufferDesc;
            bufferDesc.Size           = sizeof(Matrix4x4);
            bufferDesc.Usage          = Usages::DYNAMIC;
            bufferDesc.BindFlags      = BindFlags::UNIFORM_BUFFER;
            bufferDesc.CPUAccessFlags = CpuAccessFlags::WRITE;
            m_Device->CreateBuffer(bufferDesc, nullptr, &m_VBConstants);
        }
        m_PSO->GetStaticVariableByName(ShaderTypes::VERTEX, "Constants")->Set(m_VBConstants);

        // Create SRB, init Constants, and get TextureVariable
        m_SRB.Release();
        m_PSO->CreateShaderResourceBinding(&m_SRB, true);
        m_TextureVariable = m_SRB->GetVariableByName(ShaderTypes::PIXEL, "Texture");
        RYN_VERIFY(m_TextureVariable != nullptr);
    }

    void ImGuiBackend::invalidateDeviceObjects() {
        ImGuiPlatformIO& io = ImGui::GetPlatformIO();
        for (ImTextureData* imTexture : io.Textures)
            destroyTexture(imTexture);

        m_VB.Release();
        m_IB.Release();
        m_VBConstants.Release();
        m_PSO.Release();
        m_SRB.Release();
        m_TextureVariable = nullptr;
    }
}

namespace {
    TextureFormat convertTextureFormat(const ImTextureFormat imFormat) {
        switch (imFormat) {
        case ImTextureFormat_Alpha8:
            return TextureFormats::R8_UNORM;
        case ImTextureFormat_RGBA32:
            return TextureFormats::RGBA8_UNORM;
        default:
            RYN_ASSERT(false, "Unknown texture format!");
            return TextureFormats::UNKNOWN;
        }
    }

    Vector4 transformScissor(const ImVec2& displaySize, const Vector4& rect, const SurfaceTransform surfaceTransform) {
        const Vector2 a(rect.x, rect.y); // Top-Left corner
        const Vector2 b(rect.z, rect.w); // Bottom-Right corner

        switch (surfaceTransform) {
        case SurfaceTransforms::IDENTITY:
            return rect;

        case SurfaceTransforms::ROTATE_90:
            return Vector4{
                displaySize.y - b.y, // min_x = b'.x
                a.x,                 // min_y = a'.y
                displaySize.y - a.y, // max_x = a'.x
                b.x                  // max_y = b'.y
            };

        case SurfaceTransforms::ROTATE_180:
            return Vector4{
                displaySize.x - b.x, // min_x = b'.x
                displaySize.y - b.y, // min_y = b'.y
                displaySize.x - a.x, // max_x = a'.x
                displaySize.y - a.y  // max_y = a'.y
            };

        case SurfaceTransforms::ROTATE_270:
            return Vector4{
                a.y,                 // min_x = a'.x
                displaySize.x - b.x, // min_y = b'.y
                b.y,                 // max_x = b'.x
                displaySize.x - a.x  // max_y = a'.y
            };

        case SurfaceTransforms::OPTIMAL:
            RYN_VERIFY(false, "SURFACE_TRANSFORM_OPTIMAL is only valid as parameter during swapchain initialization!");
            return rect;

        case SurfaceTransforms::HORIZONTAL_MIRROR:
        case SurfaceTransforms::HORIZONTAL_MIRROR_ROTATE_90:
        case SurfaceTransforms::HORIZONTAL_MIRROR_ROTATE_180:
        case SurfaceTransforms::HORIZONTAL_MIRROR_ROTATE_270:
            RYN_VERIFY(false, "Mirror surface tranforms are not supported!");
            return rect;

        default:
            RYN_VERIFY(false, "Unknown surface transform!");
            return rect;
        }
    }

    void updateTexture(IDevice* device, IContext* context, ImTextureData* imTextureData) {
        auto* texture = static_cast<ITexture*>(imTextureData->BackendUserData);

        switch (imTextureData->Status) {
        case ImTextureStatus_WantCreate: {
            // Assert texture doesn't have backend data yet and is ID invalid
            // Something has gone terribly wrong if this isn't the case
            RYN_ASSERT(texture == nullptr && imTextureData->TexID == ImTextureID_Invalid, "Texture state incoherent!");

            TextureDesc textureDesc;
            textureDesc.Name      = "ImGui Texture";
            textureDesc.Type      = ResourceDimensions::TEX_2D;
            textureDesc.Width     = static_cast<uint32_t>(imTextureData->Width);
            textureDesc.Height    = static_cast<uint32_t>(imTextureData->Height);
            textureDesc.Format    = convertTextureFormat(imTextureData->Format);
            textureDesc.Usage     = Usages::DEFAULT;
            textureDesc.BindFlags = BindFlags::SHADER_RESOURCE;

            TextureSubResData textureSubData;
            textureSubData.pData  = imTextureData->GetPixels();
            textureSubData.Stride = static_cast<uint64_t>(imTextureData->GetPitch());

            TextureData textureData;
            textureData.pSubResources   = &textureSubData;
            textureData.NumSubresources = 1;

            device->CreateTexture(textureDesc, &textureData, &texture);
            RYN_VERIFY(texture != nullptr);
            ITextureView* textureView = texture->GetDefaultView(TextureViewTypes::SHADER_RESOURCE);
            RYN_VERIFY(textureView != nullptr);

            // Store texture view and texture pointers in ImGui and set state to ok
            imTextureData->SetTexID(reinterpret_cast<ImTextureID>(textureView));
            imTextureData->BackendUserData = reinterpret_cast<void*>(texture);
            imTextureData->SetStatus(ImTextureStatus_OK);
            break;
        }
        case ImTextureStatus_WantUpdates: {
            if (texture == nullptr) break;

            const graphics::Box destinationBox{
                static_cast<uint32_t>(imTextureData->UpdateRect.x),
                static_cast<uint32_t>(imTextureData->UpdateRect.x + imTextureData->UpdateRect.w),
                static_cast<uint32_t>(imTextureData->UpdateRect.y),
                static_cast<uint32_t>(imTextureData->UpdateRect.y + imTextureData->UpdateRect.h),
            };

            TextureSubResData textureSubData;
            textureSubData.pData  = imTextureData->GetPixelsAt(imTextureData->UpdateRect.x, imTextureData->UpdateRect.y);
            textureSubData.Stride = static_cast<uint64_t>(imTextureData->GetPitch());

            context->UpdateTexture(texture,
                                   0,
                                   0,
                                   destinationBox,
                                   textureSubData,
                                   ResourceStateTransitionModes::VERIFY,
                                   ResourceStateTransitionModes::TRANSITION);

            imTextureData->SetStatus(ImTextureStatus_OK);
            break;
        }
        case ImTextureStatus_WantDestroy: {
            if (imTextureData->UnusedFrames == 0) break;
            destroyTexture(imTextureData);
            break;
        }
        default:
            break;
        }
    }

    void destroyTexture(ImTextureData* imTextureData) {
        if (auto* texture = static_cast<ITexture*>(imTextureData->BackendUserData)) texture->Release();

        imTextureData->BackendUserData = nullptr;
        imTextureData->SetTexID(ImTextureID_Invalid);
        imTextureData->SetStatus(ImTextureStatus_Destroyed);
    }
}
