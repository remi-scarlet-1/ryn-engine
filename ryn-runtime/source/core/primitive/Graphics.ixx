module;
#include "Prelude.h"
#include <BlendState.h>
#include <BottomLevelAS.h>
#include <Buffer.h>
#include <BufferView.h>
#include <CommandList.h>
#include <CommandQueue.h>
#include <DepthStencilState.h>
#include <DeviceContext.h>
#include <DeviceMemory.h>
#include <DeviceObject.h>
#include <EngineFactory.h>
#include <Fence.h>
#include <Framebuffer.h>
#include <GraphicsTypes.h>
#include <InputLayout.h>
#include <PipelineResourceSignature.h>
#include <PipelineState.h>
#include <PipelineStateCache.h>
#include <Query.h>
#include <RasterizerState.h>
#include <RenderPass.h>
#include <ResourceMapping.h>
#include <Sampler.h>
#include <Shader.h>
#include <ShaderBindingTable.h>
#include <ShaderResourceBinding.h>
#include <ShaderResourceVariable.h>
#include <Texture.h>
#include <TextureView.h>
#include <TopLevelAS.h>

#include <GraphicsAccessories.hpp>
#include <MapHelper.hpp>
#include <RefCntAutoPtr.hpp>
#include <RenderDevice.h>
#include <SwapChain.h>

export module Ryntime.Core.Primitive:Graphics;

/* OTHER DILIGENT ALIASES */
export namespace ryn::graphics {
    // Shortened versions
    using IDevice    = Diligent::IRenderDevice;
    using IContext   = Diligent::IDeviceContext;
    using ISwapchain = Diligent::ISwapChain;

    template<typename T>
    using SharedRef = Diligent::RefCntAutoPtr<T>;

    template<typename T>
    using MapHelper = Diligent::MapHelper<T>;

    using TextureFormatAttribs = Diligent::TextureFormatAttribs;
    [[nodiscard]] RYN_API const TextureFormatAttribs& getTextureFormatAttribs(Diligent::TEXTURE_FORMAT format) noexcept {
        return Diligent::GetTextureFormatAttribs(format);
    }
}

/* AUTOMATICALLY GENERATED DILIGENT ALIASES */
export namespace ryn::graphics {

    /* ===== STRUCT ALIASES ===== */
    // BlendState.h
    using BlendStateDesc        = Diligent::BlendStateDesc;
    using RenderTargetBlendDesc = Diligent::RenderTargetBlendDesc;

    // BottomLevelAS.h
    using BLASBoundingBoxDesc = Diligent::BLASBoundingBoxDesc;
    using BLASTriangleDesc    = Diligent::BLASTriangleDesc;
    using BottomLevelASDesc   = Diligent::BottomLevelASDesc;
    using ScratchBufferSizes  = Diligent::ScratchBufferSizes;

    // Buffer.h
    using BufferData             = Diligent::BufferData;
    using BufferDesc             = Diligent::BufferDesc;
    using SparseBufferProperties = Diligent::SparseBufferProperties;

    // BufferView.h
    using BufferFormat   = Diligent::BufferFormat;
    using BufferViewDesc = Diligent::BufferViewDesc;

    // DepthStencilState.h
    using DepthStencilStateDesc = Diligent::DepthStencilStateDesc;
    using StencilOpDesc         = Diligent::StencilOpDesc;

    // DeviceContext.h
    using BLASBuildBoundingBoxData         = Diligent::BLASBuildBoundingBoxData;
    using BLASBuildTriangleData            = Diligent::BLASBuildTriangleData;
    using BeginRenderPassAttribs           = Diligent::BeginRenderPassAttribs;
    using BindSparseResourceMemoryAttribs  = Diligent::BindSparseResourceMemoryAttribs;
    using BuildBLASAttribs                 = Diligent::BuildBLASAttribs;
    using BuildTLASAttribs                 = Diligent::BuildTLASAttribs;
    using CopyBLASAttribs                  = Diligent::CopyBLASAttribs;
    using CopyTLASAttribs                  = Diligent::CopyTLASAttribs;
    using CopyTextureAttribs               = Diligent::CopyTextureAttribs;
    using DeviceContextCommandCounters     = Diligent::DeviceContextCommandCounters;
    using DeviceContextDesc                = Diligent::DeviceContextDesc;
    using DeviceContextStats               = Diligent::DeviceContextStats;
    using DispatchComputeAttribs           = Diligent::DispatchComputeAttribs;
    using DispatchComputeIndirectAttribs   = Diligent::DispatchComputeIndirectAttribs;
    using DispatchTileAttribs              = Diligent::DispatchTileAttribs;
    using DrawAttribs                      = Diligent::DrawAttribs;
    using DrawIndexedAttribs               = Diligent::DrawIndexedAttribs;
    using DrawIndexedIndirectAttribs       = Diligent::DrawIndexedIndirectAttribs;
    using DrawIndirectAttribs              = Diligent::DrawIndirectAttribs;
    using DrawMeshAttribs                  = Diligent::DrawMeshAttribs;
    using DrawMeshIndirectAttribs          = Diligent::DrawMeshIndirectAttribs;
    using InstanceMatrix                   = Diligent::InstanceMatrix;
    using MultiDrawAttribs                 = Diligent::MultiDrawAttribs;
    using MultiDrawIndexedAttribs          = Diligent::MultiDrawIndexedAttribs;
    using MultiDrawIndexedItem             = Diligent::MultiDrawIndexedItem;
    using MultiDrawItem                    = Diligent::MultiDrawItem;
    using Rect                             = Diligent::Rect;
    using ResolveTextureSubresourceAttribs = Diligent::ResolveTextureSubresourceAttribs;
    using SetRenderTargetsAttribs          = Diligent::SetRenderTargetsAttribs;
    using SparseBufferMemoryBindInfo       = Diligent::SparseBufferMemoryBindInfo;
    using SparseBufferMemoryBindRange      = Diligent::SparseBufferMemoryBindRange;
    using SparseTextureMemoryBindInfo      = Diligent::SparseTextureMemoryBindInfo;
    using SparseTextureMemoryBindRange     = Diligent::SparseTextureMemoryBindRange;
    using StateTransitionDesc              = Diligent::StateTransitionDesc;
    using TLASBuildInstanceData            = Diligent::TLASBuildInstanceData;
    using TraceRaysAttribs                 = Diligent::TraceRaysAttribs;
    using TraceRaysIndirectAttribs         = Diligent::TraceRaysIndirectAttribs;
    using UpdateIndirectRTBufferAttribs    = Diligent::UpdateIndirectRTBufferAttribs;
    using Viewport                         = Diligent::Viewport;
    using WriteBLASCompactedSizeAttribs    = Diligent::WriteBLASCompactedSizeAttribs;
    using WriteTLASCompactedSizeAttribs    = Diligent::WriteTLASCompactedSizeAttribs;

    // DeviceMemory.h
    using DeviceMemoryCreateInfo = Diligent::DeviceMemoryCreateInfo;
    using DeviceMemoryDesc       = Diligent::DeviceMemoryDesc;

    // EngineFactory.h
    using DearchiverCreateInfo = Diligent::DearchiverCreateInfo;

    // Fence.h
    using FenceDesc = Diligent::FenceDesc;

    // Framebuffer.h
    using FramebufferDesc = Diligent::FramebufferDesc;

    // GraphicsTypes.h
    using AdapterMemoryInfo             = Diligent::AdapterMemoryInfo;
    using Box                           = Diligent::Box;
    using BufferProperties              = Diligent::BufferProperties;
    using CommandQueueInfo              = Diligent::CommandQueueInfo;
    using ComputeShaderProperties       = Diligent::ComputeShaderProperties;
    using DepthStencilClearValue        = Diligent::DepthStencilClearValue;
    using DeviceFeatures                = Diligent::DeviceFeatures;
    using DeviceFeaturesVk              = Diligent::DeviceFeaturesVk;
    using DeviceObjectAttribs           = Diligent::DeviceObjectAttribs;
    using DisplayModeAttribs            = Diligent::DisplayModeAttribs;
    using DrawCommandProperties         = Diligent::DrawCommandProperties;
    using EngineCreateInfo              = Diligent::EngineCreateInfo;
    using EngineD3D11CreateInfo         = Diligent::EngineD3D11CreateInfo;
    using EngineD3D12CreateInfo         = Diligent::EngineD3D12CreateInfo;
    using EngineGLCreateInfo            = Diligent::EngineGLCreateInfo;
    using EngineMtlCreateInfo           = Diligent::EngineMtlCreateInfo;
    using EngineVkCreateInfo            = Diligent::EngineVkCreateInfo;
    using EngineWebGPUCreateInfo        = Diligent::EngineWebGPUCreateInfo;
    using FullScreenModeDesc            = Diligent::FullScreenModeDesc;
    using GraphicsAdapterInfo           = Diligent::GraphicsAdapterInfo;
    using ImmediateContextCreateInfo    = Diligent::ImmediateContextCreateInfo;
    using MeshShaderProperties          = Diligent::MeshShaderProperties;
    using NDCAttribs                    = Diligent::NDCAttribs;
    using OpenXRAttribs                 = Diligent::OpenXRAttribs;
    using OptimizedClearValue           = Diligent::OptimizedClearValue;
    using RayTracingProperties          = Diligent::RayTracingProperties;
    using RenderDeviceInfo              = Diligent::RenderDeviceInfo;
    using RenderDeviceShaderVersionInfo = Diligent::RenderDeviceShaderVersionInfo;
    using SamplerProperties             = Diligent::SamplerProperties;
    using ShadingRateMode               = Diligent::ShadingRateMode;
    using ShadingRateProperties         = Diligent::ShadingRateProperties;
    using SparseResourceProperties      = Diligent::SparseResourceProperties;
    using SparseTextureFormatInfo       = Diligent::SparseTextureFormatInfo;
    using SwapChainDesc                 = Diligent::SwapChainDesc;
    using TextureFormatAttribs          = Diligent::TextureFormatAttribs;
    using TextureFormatInfo             = Diligent::TextureFormatInfo;
    using TextureFormatInfoExt          = Diligent::TextureFormatInfoExt;
    using TextureProperties             = Diligent::TextureProperties;
    using Version                       = Diligent::Version;
    using VulkanDescriptorPoolSize      = Diligent::VulkanDescriptorPoolSize;
    using WaveOpProperties              = Diligent::WaveOpProperties;

    // InputLayout.h
    using InputLayoutDesc = Diligent::InputLayoutDesc;
    using LayoutElement   = Diligent::LayoutElement;

    // PipelineResourceSignature.h
    using ImmutableSamplerDesc          = Diligent::ImmutableSamplerDesc;
    using PipelineResourceDesc          = Diligent::PipelineResourceDesc;
    using PipelineResourceSignatureDesc = Diligent::PipelineResourceSignatureDesc;
    using WebGPUResourceAttribs         = Diligent::WebGPUResourceAttribs;

    // PipelineStateCache.h
    using PipelineStateCacheCreateInfo = Diligent::PipelineStateCacheCreateInfo;
    using PipelineStateCacheDesc       = Diligent::PipelineStateCacheDesc;

    // PipelineState.h
    using ComputePipelineStateCreateInfo     = Diligent::ComputePipelineStateCreateInfo;
    using GraphicsPipelineDesc               = Diligent::GraphicsPipelineDesc;
    using GraphicsPipelineStateCreateInfo    = Diligent::GraphicsPipelineStateCreateInfo;
    using PipelineResourceLayoutDesc         = Diligent::PipelineResourceLayoutDesc;
    using PipelineStateCreateInfo            = Diligent::PipelineStateCreateInfo;
    using PipelineStateDesc                  = Diligent::PipelineStateDesc;
    using RayTracingGeneralShaderGroup       = Diligent::RayTracingGeneralShaderGroup;
    using RayTracingPipelineDesc             = Diligent::RayTracingPipelineDesc;
    using RayTracingPipelineStateCreateInfo  = Diligent::RayTracingPipelineStateCreateInfo;
    using RayTracingProceduralHitShaderGroup = Diligent::RayTracingProceduralHitShaderGroup;
    using RayTracingTriangleHitShaderGroup   = Diligent::RayTracingTriangleHitShaderGroup;
    using SampleDesc                         = Diligent::SampleDesc;
    using ShaderResourceVariableDesc         = Diligent::ShaderResourceVariableDesc;
    using TilePipelineDesc                   = Diligent::TilePipelineDesc;
    using TilePipelineStateCreateInfo        = Diligent::TilePipelineStateCreateInfo;

    // Query.h
    using QueryDataBinaryOcclusion    = Diligent::QueryDataBinaryOcclusion;
    using QueryDataDuration           = Diligent::QueryDataDuration;
    using QueryDataOcclusion          = Diligent::QueryDataOcclusion;
    using QueryDataPipelineStatistics = Diligent::QueryDataPipelineStatistics;
    using QueryDataTimestamp          = Diligent::QueryDataTimestamp;
    using QueryDesc                   = Diligent::QueryDesc;

    // RasterizerState.h
    using RasterizerStateDesc = Diligent::RasterizerStateDesc;

    // RenderPass.h
    using AttachmentReference      = Diligent::AttachmentReference;
    using RenderPassAttachmentDesc = Diligent::RenderPassAttachmentDesc;
    using RenderPassDesc           = Diligent::RenderPassDesc;
    using ShadingRateAttachment    = Diligent::ShadingRateAttachment;
    using SubpassDependencyDesc    = Diligent::SubpassDependencyDesc;
    using SubpassDesc              = Diligent::SubpassDesc;

    // ResourceMapping.h
    using ResourceMappingCreateInfo = Diligent::ResourceMappingCreateInfo;
    using ResourceMappingEntry      = Diligent::ResourceMappingEntry;

    // Sampler.h
    using SamplerDesc = Diligent::SamplerDesc;

    // ShaderBindingTable.h
    using ShaderBindingTableDesc = Diligent::ShaderBindingTableDesc;

    // Shader.h
    using ShaderCreateInfo   = Diligent::ShaderCreateInfo;
    using ShaderDesc         = Diligent::ShaderDesc;
    using ShaderMacro        = Diligent::ShaderMacro;
    using ShaderMacroArray   = Diligent::ShaderMacroArray;
    using ShaderResourceDesc = Diligent::ShaderResourceDesc;

    // Texture.h
    using MappedTextureSubresource = Diligent::MappedTextureSubresource;
    using SparseTextureProperties  = Diligent::SparseTextureProperties;
    using TextureData              = Diligent::TextureData;
    using TextureDesc              = Diligent::TextureDesc;
    using TextureSubResData        = Diligent::TextureSubResData;

    // TextureView.h
    using TextureComponentMapping = Diligent::TextureComponentMapping;
    using TextureViewDesc         = Diligent::TextureViewDesc;

    // TopLevelAS.h
    using TLASBuildInfo    = Diligent::TLASBuildInfo;
    using TLASInstanceDesc = Diligent::TLASInstanceDesc;
    using TopLevelASDesc   = Diligent::TopLevelASDesc;

    /* ===== INTERFACE ALIASES ===== */
    // BottomLevelAS.h
    using IBottomLevelAS = Diligent::IBottomLevelAS;

    // Buffer.h
    using IBuffer = Diligent::IBuffer;

    // BufferView.h
    using IBufferView = Diligent::IBufferView;

    // CommandQueue.h
    using ICommandQueue = Diligent::ICommandQueue;

    // DeviceMemory.h
    using IDeviceMemory = Diligent::IDeviceMemory;

    // DeviceObject.h
    using IDeviceObject = Diligent::IDeviceObject;

    // EngineFactory.h
    using IEngineFactory = Diligent::IEngineFactory;

    // Fence.h
    using IFence = Diligent::IFence;

    // PipelineResourceSignature.h
    using IPipelineResourceSignature = Diligent::IPipelineResourceSignature;

    // PipelineStateCache.h
    using IPipelineStateCache = Diligent::IPipelineStateCache;

    // PipelineState.h
    using IPipelineState = Diligent::IPipelineState;

    // Query.h
    using IQuery = Diligent::IQuery;

    // ResourceMapping.h
    using IResourceMapping = Diligent::IResourceMapping;

    // Sampler.h
    using ISampler = Diligent::ISampler;

    // ShaderBindingTable.h
    using IShaderBindingTable = Diligent::IShaderBindingTable;

    // Shader.h
    using IShader                         = Diligent::IShader;
    using IShaderSourceInputStreamFactory = Diligent::IShaderSourceInputStreamFactory;

    // ShaderResourceBinding.h
    using IShaderResourceBinding = Diligent::IShaderResourceBinding;

    // ShaderResourceVariable.h
    using IShaderResourceVariable = Diligent::IShaderResourceVariable;

    // Texture.h
    using ITexture = Diligent::ITexture;

    // TextureView.h
    using ITextureView = Diligent::ITextureView;

    // TopLevelAS.h
    using ITopLevelAS = Diligent::ITopLevelAS;

    /* ===== ENUM ALIASES ===== */
    // BlendState.h
    using BlendFactor = Diligent::BLEND_FACTOR;
    namespace BlendFactors {
        inline constexpr BlendFactor UNDEFINED        = Diligent::BLEND_FACTOR_UNDEFINED;
        inline constexpr BlendFactor ZERO             = Diligent::BLEND_FACTOR_ZERO;
        inline constexpr BlendFactor ONE              = Diligent::BLEND_FACTOR_ONE;
        inline constexpr BlendFactor SRC_COLOR        = Diligent::BLEND_FACTOR_SRC_COLOR;
        inline constexpr BlendFactor INV_SRC_COLOR    = Diligent::BLEND_FACTOR_INV_SRC_COLOR;
        inline constexpr BlendFactor SRC_ALPHA        = Diligent::BLEND_FACTOR_SRC_ALPHA;
        inline constexpr BlendFactor INV_SRC_ALPHA    = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
        inline constexpr BlendFactor DEST_ALPHA       = Diligent::BLEND_FACTOR_DEST_ALPHA;
        inline constexpr BlendFactor INV_DEST_ALPHA   = Diligent::BLEND_FACTOR_INV_DEST_ALPHA;
        inline constexpr BlendFactor DEST_COLOR       = Diligent::BLEND_FACTOR_DEST_COLOR;
        inline constexpr BlendFactor INV_DEST_COLOR   = Diligent::BLEND_FACTOR_INV_DEST_COLOR;
        inline constexpr BlendFactor SRC_ALPHA_SAT    = Diligent::BLEND_FACTOR_SRC_ALPHA_SAT;
        inline constexpr BlendFactor BLEND_FACTOR     = Diligent::BLEND_FACTOR_BLEND_FACTOR;
        inline constexpr BlendFactor INV_BLEND_FACTOR = Diligent::BLEND_FACTOR_INV_BLEND_FACTOR;
        inline constexpr BlendFactor SRC1_COLOR       = Diligent::BLEND_FACTOR_SRC1_COLOR;
        inline constexpr BlendFactor INV_SRC1_COLOR   = Diligent::BLEND_FACTOR_INV_SRC1_COLOR;
        inline constexpr BlendFactor SRC1_ALPHA       = Diligent::BLEND_FACTOR_SRC1_ALPHA;
        inline constexpr BlendFactor INV_SRC1_ALPHA   = Diligent::BLEND_FACTOR_INV_SRC1_ALPHA;
        inline constexpr BlendFactor NUM_FACTORS      = Diligent::BLEND_FACTOR_NUM_FACTORS;
    }

    using BlendOperation = Diligent::BLEND_OPERATION;
    namespace BlendOperations {
        inline constexpr BlendOperation UNDEFINED      = Diligent::BLEND_OPERATION_UNDEFINED;
        inline constexpr BlendOperation ADD            = Diligent::BLEND_OPERATION_ADD;
        inline constexpr BlendOperation SUBTRACT       = Diligent::BLEND_OPERATION_SUBTRACT;
        inline constexpr BlendOperation REV_SUBTRACT   = Diligent::BLEND_OPERATION_REV_SUBTRACT;
        inline constexpr BlendOperation MIN            = Diligent::BLEND_OPERATION_MIN;
        inline constexpr BlendOperation MAX            = Diligent::BLEND_OPERATION_MAX;
        inline constexpr BlendOperation NUM_OPERATIONS = Diligent::BLEND_OPERATION_NUM_OPERATIONS;
    }

    using ColorMask = Diligent::COLOR_MASK;
    namespace ColorMasks {
        inline constexpr ColorMask NONE  = Diligent::COLOR_MASK_NONE;
        inline constexpr ColorMask RED   = Diligent::COLOR_MASK_RED;
        inline constexpr ColorMask GREEN = Diligent::COLOR_MASK_GREEN;
        inline constexpr ColorMask BLUE  = Diligent::COLOR_MASK_BLUE;
        inline constexpr ColorMask ALPHA = Diligent::COLOR_MASK_ALPHA;
        inline constexpr ColorMask RGB   = Diligent::COLOR_MASK_RGB;
        inline constexpr ColorMask ALL   = Diligent::COLOR_MASK_ALL;
    }

    using LogicOperation = Diligent::LOGIC_OPERATION;
    namespace LogicOperations {
        inline constexpr LogicOperation CLEAR          = Diligent::LOGIC_OP_CLEAR;
        inline constexpr LogicOperation SET            = Diligent::LOGIC_OP_SET;
        inline constexpr LogicOperation COPY           = Diligent::LOGIC_OP_COPY;
        inline constexpr LogicOperation COPY_INVERTED  = Diligent::LOGIC_OP_COPY_INVERTED;
        inline constexpr LogicOperation NOOP           = Diligent::LOGIC_OP_NOOP;
        inline constexpr LogicOperation INVERT         = Diligent::LOGIC_OP_INVERT;
        inline constexpr LogicOperation AND            = Diligent::LOGIC_OP_AND;
        inline constexpr LogicOperation NAND           = Diligent::LOGIC_OP_NAND;
        inline constexpr LogicOperation OR             = Diligent::LOGIC_OP_OR;
        inline constexpr LogicOperation NOR            = Diligent::LOGIC_OP_NOR;
        inline constexpr LogicOperation XOR            = Diligent::LOGIC_OP_XOR;
        inline constexpr LogicOperation EQUIV          = Diligent::LOGIC_OP_EQUIV;
        inline constexpr LogicOperation AND_REVERSE    = Diligent::LOGIC_OP_AND_REVERSE;
        inline constexpr LogicOperation AND_INVERTED   = Diligent::LOGIC_OP_AND_INVERTED;
        inline constexpr LogicOperation OR_REVERSE     = Diligent::LOGIC_OP_OR_REVERSE;
        inline constexpr LogicOperation OR_INVERTED    = Diligent::LOGIC_OP_OR_INVERTED;
        inline constexpr LogicOperation NUM_OPERATIONS = Diligent::LOGIC_OP_NUM_OPERATIONS;
    }

    // BottomLevelAS.h
    using RaytracingBuildAsFlag = Diligent::RAYTRACING_BUILD_AS_FLAGS;
    namespace RaytracingBuildAsFlags {
        inline constexpr RaytracingBuildAsFlag NONE              = Diligent::RAYTRACING_BUILD_AS_NONE;
        inline constexpr RaytracingBuildAsFlag ALLOW_UPDATE      = Diligent::RAYTRACING_BUILD_AS_ALLOW_UPDATE;
        inline constexpr RaytracingBuildAsFlag ALLOW_COMPACTION  = Diligent::RAYTRACING_BUILD_AS_ALLOW_COMPACTION;
        inline constexpr RaytracingBuildAsFlag PREFER_FAST_TRACE = Diligent::RAYTRACING_BUILD_AS_PREFER_FAST_TRACE;
        inline constexpr RaytracingBuildAsFlag PREFER_FAST_BUILD = Diligent::RAYTRACING_BUILD_AS_PREFER_FAST_BUILD;
        inline constexpr RaytracingBuildAsFlag LOW_MEMORY        = Diligent::RAYTRACING_BUILD_AS_LOW_MEMORY;
        inline constexpr RaytracingBuildAsFlag LAST              = Diligent::RAYTRACING_BUILD_AS_FLAG_LAST;
    }

    // Buffer.h
    using BufferMode = Diligent::BUFFER_MODE;
    namespace BufferModes {
        inline constexpr BufferMode UNDEFINED  = Diligent::BUFFER_MODE_UNDEFINED;
        inline constexpr BufferMode FORMATTED  = Diligent::BUFFER_MODE_FORMATTED;
        inline constexpr BufferMode STRUCTURED = Diligent::BUFFER_MODE_STRUCTURED;
        inline constexpr BufferMode RAW        = Diligent::BUFFER_MODE_RAW;
        inline constexpr BufferMode NUM_MODES  = Diligent::BUFFER_MODE_NUM_MODES;
    }

    using MiscBufferFlag = Diligent::MISC_BUFFER_FLAGS;
    namespace MiscBufferFlags {
        inline constexpr MiscBufferFlag NONE            = Diligent::MISC_BUFFER_FLAG_NONE;
        inline constexpr MiscBufferFlag SPARSE_ALIASING = Diligent::MISC_BUFFER_FLAG_SPARSE_ALIASING;
    }

    // DepthStencilState.h
    using StencilOp = Diligent::STENCIL_OP;
    namespace StencilOps {
        inline constexpr StencilOp UNDEFINED = Diligent::STENCIL_OP_UNDEFINED;
        inline constexpr StencilOp KEEP      = Diligent::STENCIL_OP_KEEP;
        inline constexpr StencilOp ZERO      = Diligent::STENCIL_OP_ZERO;
        inline constexpr StencilOp REPLACE   = Diligent::STENCIL_OP_REPLACE;
        inline constexpr StencilOp INCR_SAT  = Diligent::STENCIL_OP_INCR_SAT;
        inline constexpr StencilOp DECR_SAT  = Diligent::STENCIL_OP_DECR_SAT;
        inline constexpr StencilOp INVERT    = Diligent::STENCIL_OP_INVERT;
        inline constexpr StencilOp INCR_WRAP = Diligent::STENCIL_OP_INCR_WRAP;
        inline constexpr StencilOp DECR_WRAP = Diligent::STENCIL_OP_DECR_WRAP;
        inline constexpr StencilOp NUM_OPS   = Diligent::STENCIL_OP_NUM_OPS;
    }

    // DeviceContext.h
    using DrawFlag = Diligent::DRAW_FLAGS;
    namespace DrawFlags {
        inline constexpr DrawFlag NONE                            = Diligent::DRAW_FLAG_NONE;
        inline constexpr DrawFlag VERIFY_STATES                   = Diligent::DRAW_FLAG_VERIFY_STATES;
        inline constexpr DrawFlag VERIFY_DRAW_ATTRIBS             = Diligent::DRAW_FLAG_VERIFY_DRAW_ATTRIBS;
        inline constexpr DrawFlag VERIFY_ALL                      = Diligent::DRAW_FLAG_VERIFY_ALL;
        inline constexpr DrawFlag DYNAMIC_RESOURCE_BUFFERS_INTACT = Diligent::DRAW_FLAG_DYNAMIC_RESOURCE_BUFFERS_INTACT;
        inline constexpr DrawFlag INLINE_CONSTANTS_INTACT         = Diligent::DRAW_FLAG_INLINE_CONSTANTS_INTACT;
        inline constexpr DrawFlag DYNAMIC_BUFFERS_AND_INLINE_CONSTANTS_INTACT = Diligent::DRAW_FLAG_DYNAMIC_BUFFERS_AND_INLINE_CONSTANTS_INTACT;
    }

    using ResourceStateTransitionMode = Diligent::RESOURCE_STATE_TRANSITION_MODE;
    namespace ResourceStateTransitionModes {
        inline constexpr ResourceStateTransitionMode NONE       = Diligent::RESOURCE_STATE_TRANSITION_MODE_NONE;
        inline constexpr ResourceStateTransitionMode TRANSITION = Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
        inline constexpr ResourceStateTransitionMode VERIFY     = Diligent::RESOURCE_STATE_TRANSITION_MODE_VERIFY;
    }

    using ClearDepthStencilFlag = Diligent::CLEAR_DEPTH_STENCIL_FLAGS;
    namespace ClearDepthStencilFlags {
        inline constexpr ClearDepthStencilFlag FLAG_NONE    = Diligent::CLEAR_DEPTH_FLAG_NONE;
        inline constexpr ClearDepthStencilFlag FLAG         = Diligent::CLEAR_DEPTH_FLAG;
        inline constexpr ClearDepthStencilFlag STENCIL_FLAG = Diligent::CLEAR_STENCIL_FLAG;
    }

    using SetVertexBuffersFlag = Diligent::SET_VERTEX_BUFFERS_FLAGS;
    namespace SetVertexBuffersFlags {
        inline constexpr SetVertexBuffersFlag NONE  = Diligent::SET_VERTEX_BUFFERS_FLAG_NONE;
        inline constexpr SetVertexBuffersFlag RESET = Diligent::SET_VERTEX_BUFFERS_FLAG_RESET;
    }

    using RaytracingInstanceFlag = Diligent::RAYTRACING_INSTANCE_FLAGS;
    namespace RaytracingInstanceFlags {
        inline constexpr RaytracingInstanceFlag NONE                         = Diligent::RAYTRACING_INSTANCE_NONE;
        inline constexpr RaytracingInstanceFlag TRIANGLE_FACING_CULL_DISABLE = Diligent::RAYTRACING_INSTANCE_TRIANGLE_FACING_CULL_DISABLE;
        inline constexpr RaytracingInstanceFlag TRIANGLE_FRONT_COUNTERCLOCKWISE = Diligent::RAYTRACING_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE;
        inline constexpr RaytracingInstanceFlag FORCE_OPAQUE    = Diligent::RAYTRACING_INSTANCE_FORCE_OPAQUE;
        inline constexpr RaytracingInstanceFlag FORCE_NO_OPAQUE = Diligent::RAYTRACING_INSTANCE_FORCE_NO_OPAQUE;
        inline constexpr RaytracingInstanceFlag LAST            = Diligent::RAYTRACING_INSTANCE_FLAG_LAST;
    }

    using CopyAsMode = Diligent::COPY_AS_MODE;
    namespace CopyAsModes {
        inline constexpr CopyAsMode CLONE   = Diligent::COPY_AS_MODE_CLONE;
        inline constexpr CopyAsMode COMPACT = Diligent::COPY_AS_MODE_COMPACT;
        inline constexpr CopyAsMode LAST    = Diligent::COPY_AS_MODE_LAST;
    }

    using RaytracingGeometryFlag = Diligent::RAYTRACING_GEOMETRY_FLAGS;
    namespace RaytracingGeometryFlags {
        inline constexpr RaytracingGeometryFlag NONE   = Diligent::RAYTRACING_GEOMETRY_FLAG_NONE;
        inline constexpr RaytracingGeometryFlag OPAQUE = Diligent::RAYTRACING_GEOMETRY_FLAG_OPAQUE;
        inline constexpr RaytracingGeometryFlag NO_DUPLICATE_ANY_HIT_INVOCATION = Diligent::RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANY_HIT_INVOCATION;
        inline constexpr RaytracingGeometryFlag LAST = Diligent::RAYTRACING_GEOMETRY_FLAG_LAST;
    }

    using StateTransitionFlag = Diligent::STATE_TRANSITION_FLAGS;
    namespace StateTransitionFlags {
        inline constexpr StateTransitionFlag NONE            = Diligent::STATE_TRANSITION_FLAG_NONE;
        inline constexpr StateTransitionFlag UPDATE_STATE    = Diligent::STATE_TRANSITION_FLAG_UPDATE_STATE;
        inline constexpr StateTransitionFlag DISCARD_CONTENT = Diligent::STATE_TRANSITION_FLAG_DISCARD_CONTENT;
        inline constexpr StateTransitionFlag ALIASING        = Diligent::STATE_TRANSITION_FLAG_ALIASING;
    }

    // DeviceMemory.h
    using DeviceMemoryType = Diligent::DEVICE_MEMORY_TYPE;
    namespace DeviceMemoryTypes {
        inline constexpr DeviceMemoryType UNDEFINED = Diligent::DEVICE_MEMORY_TYPE_UNDEFINED;
        inline constexpr DeviceMemoryType SPARSE    = Diligent::DEVICE_MEMORY_TYPE_SPARSE;
    }

    // Fence.h
    using FenceType = Diligent::FENCE_TYPE;
    namespace FenceTypes {
        inline constexpr FenceType CPU_WAIT_ONLY = Diligent::FENCE_TYPE_CPU_WAIT_ONLY;
        inline constexpr FenceType GENERAL       = Diligent::FENCE_TYPE_GENERAL;
        inline constexpr FenceType LAST          = Diligent::FENCE_TYPE_LAST;
    }

    // GraphicsTypes.h
    using ValueType = Diligent::VALUE_TYPE;
    namespace ValueTypes {
        inline constexpr ValueType VT_UNDEFINED = Diligent::VT_UNDEFINED;
        inline constexpr ValueType VT_INT8      = Diligent::VT_INT8;
        inline constexpr ValueType VT_INT16     = Diligent::VT_INT16;
        inline constexpr ValueType VT_INT32     = Diligent::VT_INT32;
        inline constexpr ValueType VT_UINT8     = Diligent::VT_UINT8;
        inline constexpr ValueType VT_UINT16    = Diligent::VT_UINT16;
        inline constexpr ValueType VT_UINT32    = Diligent::VT_UINT32;
        inline constexpr ValueType VT_FLOAT16   = Diligent::VT_FLOAT16;
        inline constexpr ValueType VT_FLOAT32   = Diligent::VT_FLOAT32;
        inline constexpr ValueType VT_FLOAT64   = Diligent::VT_FLOAT64;
        inline constexpr ValueType VT_NUM_TYPES = Diligent::VT_NUM_TYPES;
    }

    using ShaderType = Diligent::SHADER_TYPE;
    namespace ShaderTypes {
        inline constexpr ShaderType UNKNOWN          = Diligent::SHADER_TYPE_UNKNOWN;
        inline constexpr ShaderType VERTEX           = Diligent::SHADER_TYPE_VERTEX;
        inline constexpr ShaderType PIXEL            = Diligent::SHADER_TYPE_PIXEL;
        inline constexpr ShaderType GEOMETRY         = Diligent::SHADER_TYPE_GEOMETRY;
        inline constexpr ShaderType HULL             = Diligent::SHADER_TYPE_HULL;
        inline constexpr ShaderType DOMAIN           = Diligent::SHADER_TYPE_DOMAIN;
        inline constexpr ShaderType COMPUTE          = Diligent::SHADER_TYPE_COMPUTE;
        inline constexpr ShaderType AMPLIFICATION    = Diligent::SHADER_TYPE_AMPLIFICATION;
        inline constexpr ShaderType MESH             = Diligent::SHADER_TYPE_MESH;
        inline constexpr ShaderType RAY_GEN          = Diligent::SHADER_TYPE_RAY_GEN;
        inline constexpr ShaderType RAY_MISS         = Diligent::SHADER_TYPE_RAY_MISS;
        inline constexpr ShaderType RAY_CLOSEST_HIT  = Diligent::SHADER_TYPE_RAY_CLOSEST_HIT;
        inline constexpr ShaderType RAY_ANY_HIT      = Diligent::SHADER_TYPE_RAY_ANY_HIT;
        inline constexpr ShaderType RAY_INTERSECTION = Diligent::SHADER_TYPE_RAY_INTERSECTION;
        inline constexpr ShaderType CALLABLE         = Diligent::SHADER_TYPE_CALLABLE;
        inline constexpr ShaderType TILE             = Diligent::SHADER_TYPE_TILE;
        inline constexpr ShaderType LAST             = Diligent::SHADER_TYPE_LAST;
        inline constexpr ShaderType VS_PS            = Diligent::SHADER_TYPE_VS_PS;
        inline constexpr ShaderType ALL_GRAPHICS     = Diligent::SHADER_TYPE_ALL_GRAPHICS;
        inline constexpr ShaderType ALL_MESH         = Diligent::SHADER_TYPE_ALL_MESH;
        inline constexpr ShaderType ALL_RAY_TRACING  = Diligent::SHADER_TYPE_ALL_RAY_TRACING;
        inline constexpr ShaderType ALL              = Diligent::SHADER_TYPE_ALL;
    }

    using BindFlag = Diligent::BIND_FLAGS;
    namespace BindFlags {
        inline constexpr BindFlag NONE               = Diligent::BIND_NONE;
        inline constexpr BindFlag VERTEX_BUFFER      = Diligent::BIND_VERTEX_BUFFER;
        inline constexpr BindFlag INDEX_BUFFER       = Diligent::BIND_INDEX_BUFFER;
        inline constexpr BindFlag UNIFORM_BUFFER     = Diligent::BIND_UNIFORM_BUFFER;
        inline constexpr BindFlag SHADER_RESOURCE    = Diligent::BIND_SHADER_RESOURCE;
        inline constexpr BindFlag STREAM_OUTPUT      = Diligent::BIND_STREAM_OUTPUT;
        inline constexpr BindFlag RENDER_TARGET      = Diligent::BIND_RENDER_TARGET;
        inline constexpr BindFlag DEPTH_STENCIL      = Diligent::BIND_DEPTH_STENCIL;
        inline constexpr BindFlag UNORDERED_ACCESS   = Diligent::BIND_UNORDERED_ACCESS;
        inline constexpr BindFlag INDIRECT_DRAW_ARGS = Diligent::BIND_INDIRECT_DRAW_ARGS;
        inline constexpr BindFlag INPUT_ATTACHMENT   = Diligent::BIND_INPUT_ATTACHMENT;
        inline constexpr BindFlag RAY_TRACING        = Diligent::BIND_RAY_TRACING;
        inline constexpr BindFlag SHADING_RATE       = Diligent::BIND_SHADING_RATE;
        inline constexpr BindFlag LAST               = Diligent::BIND_FLAG_LAST;
    }

    using Usage = Diligent::USAGE;
    namespace Usages {
        inline constexpr Usage IMMUTABLE  = Diligent::USAGE_IMMUTABLE;
        inline constexpr Usage DEFAULT    = Diligent::USAGE_DEFAULT;
        inline constexpr Usage DYNAMIC    = Diligent::USAGE_DYNAMIC;
        inline constexpr Usage STAGING    = Diligent::USAGE_STAGING;
        inline constexpr Usage UNIFIED    = Diligent::USAGE_UNIFIED;
        inline constexpr Usage SPARSE     = Diligent::USAGE_SPARSE;
        inline constexpr Usage NUM_USAGES = Diligent::USAGE_NUM_USAGES;
    }

    using CpuAccessFlag = Diligent::CPU_ACCESS_FLAGS;
    namespace CpuAccessFlags {
        inline constexpr CpuAccessFlag NONE  = Diligent::CPU_ACCESS_NONE;
        inline constexpr CpuAccessFlag READ  = Diligent::CPU_ACCESS_READ;
        inline constexpr CpuAccessFlag WRITE = Diligent::CPU_ACCESS_WRITE;
        inline constexpr CpuAccessFlag LAST  = Diligent::CPU_ACCESS_FLAG_LAST;
    }

    using MapType = Diligent::MAP_TYPE;
    namespace MapTypes {
        inline constexpr MapType READ       = Diligent::MAP_READ;
        inline constexpr MapType WRITE      = Diligent::MAP_WRITE;
        inline constexpr MapType READ_WRITE = Diligent::MAP_READ_WRITE;
    }

    using MapFlag = Diligent::MAP_FLAGS;
    namespace MapFlags {
        inline constexpr MapFlag NONE         = Diligent::MAP_FLAG_NONE;
        inline constexpr MapFlag DO_NOT_WAIT  = Diligent::MAP_FLAG_DO_NOT_WAIT;
        inline constexpr MapFlag DISCARD      = Diligent::MAP_FLAG_DISCARD;
        inline constexpr MapFlag NO_OVERWRITE = Diligent::MAP_FLAG_NO_OVERWRITE;
    }

    using ResourceDimension = Diligent::RESOURCE_DIMENSION;
    namespace ResourceDimensions {
        inline constexpr ResourceDimension UNDEFINED      = Diligent::RESOURCE_DIM_UNDEFINED;
        inline constexpr ResourceDimension BUFFER         = Diligent::RESOURCE_DIM_BUFFER;
        inline constexpr ResourceDimension TEX_1D         = Diligent::RESOURCE_DIM_TEX_1D;
        inline constexpr ResourceDimension TEX_1D_ARRAY   = Diligent::RESOURCE_DIM_TEX_1D_ARRAY;
        inline constexpr ResourceDimension TEX_2D         = Diligent::RESOURCE_DIM_TEX_2D;
        inline constexpr ResourceDimension TEX_2D_ARRAY   = Diligent::RESOURCE_DIM_TEX_2D_ARRAY;
        inline constexpr ResourceDimension TEX_3D         = Diligent::RESOURCE_DIM_TEX_3D;
        inline constexpr ResourceDimension TEX_CUBE       = Diligent::RESOURCE_DIM_TEX_CUBE;
        inline constexpr ResourceDimension TEX_CUBE_ARRAY = Diligent::RESOURCE_DIM_TEX_CUBE_ARRAY;
        inline constexpr ResourceDimension NUM_DIMENSIONS = Diligent::RESOURCE_DIM_NUM_DIMENSIONS;
    }

    using TextureViewType = Diligent::TEXTURE_VIEW_TYPE;
    namespace TextureViewTypes {
        inline constexpr TextureViewType UNDEFINED               = Diligent::TEXTURE_VIEW_UNDEFINED;
        inline constexpr TextureViewType SHADER_RESOURCE         = Diligent::TEXTURE_VIEW_SHADER_RESOURCE;
        inline constexpr TextureViewType RENDER_TARGET           = Diligent::TEXTURE_VIEW_RENDER_TARGET;
        inline constexpr TextureViewType DEPTH_STENCIL           = Diligent::TEXTURE_VIEW_DEPTH_STENCIL;
        inline constexpr TextureViewType READ_ONLY_DEPTH_STENCIL = Diligent::TEXTURE_VIEW_READ_ONLY_DEPTH_STENCIL;
        inline constexpr TextureViewType UNORDERED_ACCESS        = Diligent::TEXTURE_VIEW_UNORDERED_ACCESS;
        inline constexpr TextureViewType SHADING_RATE            = Diligent::TEXTURE_VIEW_SHADING_RATE;
        inline constexpr TextureViewType NUM_VIEWS               = Diligent::TEXTURE_VIEW_NUM_VIEWS;
    }

    using BufferViewType = Diligent::BUFFER_VIEW_TYPE;
    namespace BufferViewTypes {
        inline constexpr BufferViewType UNDEFINED        = Diligent::BUFFER_VIEW_UNDEFINED;
        inline constexpr BufferViewType SHADER_RESOURCE  = Diligent::BUFFER_VIEW_SHADER_RESOURCE;
        inline constexpr BufferViewType UNORDERED_ACCESS = Diligent::BUFFER_VIEW_UNORDERED_ACCESS;
        inline constexpr BufferViewType NUM_VIEWS        = Diligent::BUFFER_VIEW_NUM_VIEWS;
    }

    using TextureFormat = Diligent::TEXTURE_FORMAT;
    namespace TextureFormats {
        inline constexpr TextureFormat UNKNOWN                    = Diligent::TEX_FORMAT_UNKNOWN;
        inline constexpr TextureFormat RGBA32_TYPELESS            = Diligent::TEX_FORMAT_RGBA32_TYPELESS;
        inline constexpr TextureFormat RGBA32_FLOAT               = Diligent::TEX_FORMAT_RGBA32_FLOAT;
        inline constexpr TextureFormat RGBA32_UINT                = Diligent::TEX_FORMAT_RGBA32_UINT;
        inline constexpr TextureFormat RGBA32_SINT                = Diligent::TEX_FORMAT_RGBA32_SINT;
        inline constexpr TextureFormat RGB32_TYPELESS             = Diligent::TEX_FORMAT_RGB32_TYPELESS;
        inline constexpr TextureFormat RGB32_FLOAT                = Diligent::TEX_FORMAT_RGB32_FLOAT;
        inline constexpr TextureFormat RGB32_UINT                 = Diligent::TEX_FORMAT_RGB32_UINT;
        inline constexpr TextureFormat RGB32_SINT                 = Diligent::TEX_FORMAT_RGB32_SINT;
        inline constexpr TextureFormat RGBA16_TYPELESS            = Diligent::TEX_FORMAT_RGBA16_TYPELESS;
        inline constexpr TextureFormat RGBA16_FLOAT               = Diligent::TEX_FORMAT_RGBA16_FLOAT;
        inline constexpr TextureFormat RGBA16_UNORM               = Diligent::TEX_FORMAT_RGBA16_UNORM;
        inline constexpr TextureFormat RGBA16_UINT                = Diligent::TEX_FORMAT_RGBA16_UINT;
        inline constexpr TextureFormat RGBA16_SNORM               = Diligent::TEX_FORMAT_RGBA16_SNORM;
        inline constexpr TextureFormat RGBA16_SINT                = Diligent::TEX_FORMAT_RGBA16_SINT;
        inline constexpr TextureFormat RG32_TYPELESS              = Diligent::TEX_FORMAT_RG32_TYPELESS;
        inline constexpr TextureFormat RG32_FLOAT                 = Diligent::TEX_FORMAT_RG32_FLOAT;
        inline constexpr TextureFormat RG32_UINT                  = Diligent::TEX_FORMAT_RG32_UINT;
        inline constexpr TextureFormat RG32_SINT                  = Diligent::TEX_FORMAT_RG32_SINT;
        inline constexpr TextureFormat R32G8X24_TYPELESS          = Diligent::TEX_FORMAT_R32G8X24_TYPELESS;
        inline constexpr TextureFormat D32_FLOAT_S8X24_UINT       = Diligent::TEX_FORMAT_D32_FLOAT_S8X24_UINT;
        inline constexpr TextureFormat R32_FLOAT_X8X24_TYPELESS   = Diligent::TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        inline constexpr TextureFormat X32_TYPELESS_G8X24_UINT    = Diligent::TEX_FORMAT_X32_TYPELESS_G8X24_UINT;
        inline constexpr TextureFormat RGB10A2_TYPELESS           = Diligent::TEX_FORMAT_RGB10A2_TYPELESS;
        inline constexpr TextureFormat RGB10A2_UNORM              = Diligent::TEX_FORMAT_RGB10A2_UNORM;
        inline constexpr TextureFormat RGB10A2_UINT               = Diligent::TEX_FORMAT_RGB10A2_UINT;
        inline constexpr TextureFormat R11G11B10_FLOAT            = Diligent::TEX_FORMAT_R11G11B10_FLOAT;
        inline constexpr TextureFormat RGBA8_TYPELESS             = Diligent::TEX_FORMAT_RGBA8_TYPELESS;
        inline constexpr TextureFormat RGBA8_UNORM                = Diligent::TEX_FORMAT_RGBA8_UNORM;
        inline constexpr TextureFormat RGBA8_UNORM_SRGB           = Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB;
        inline constexpr TextureFormat RGBA8_UINT                 = Diligent::TEX_FORMAT_RGBA8_UINT;
        inline constexpr TextureFormat RGBA8_SNORM                = Diligent::TEX_FORMAT_RGBA8_SNORM;
        inline constexpr TextureFormat RGBA8_SINT                 = Diligent::TEX_FORMAT_RGBA8_SINT;
        inline constexpr TextureFormat RG16_TYPELESS              = Diligent::TEX_FORMAT_RG16_TYPELESS;
        inline constexpr TextureFormat RG16_FLOAT                 = Diligent::TEX_FORMAT_RG16_FLOAT;
        inline constexpr TextureFormat RG16_UNORM                 = Diligent::TEX_FORMAT_RG16_UNORM;
        inline constexpr TextureFormat RG16_UINT                  = Diligent::TEX_FORMAT_RG16_UINT;
        inline constexpr TextureFormat RG16_SNORM                 = Diligent::TEX_FORMAT_RG16_SNORM;
        inline constexpr TextureFormat RG16_SINT                  = Diligent::TEX_FORMAT_RG16_SINT;
        inline constexpr TextureFormat R32_TYPELESS               = Diligent::TEX_FORMAT_R32_TYPELESS;
        inline constexpr TextureFormat D32_FLOAT                  = Diligent::TEX_FORMAT_D32_FLOAT;
        inline constexpr TextureFormat R32_FLOAT                  = Diligent::TEX_FORMAT_R32_FLOAT;
        inline constexpr TextureFormat R32_UINT                   = Diligent::TEX_FORMAT_R32_UINT;
        inline constexpr TextureFormat R32_SINT                   = Diligent::TEX_FORMAT_R32_SINT;
        inline constexpr TextureFormat R24G8_TYPELESS             = Diligent::TEX_FORMAT_R24G8_TYPELESS;
        inline constexpr TextureFormat D24_UNORM_S8_UINT          = Diligent::TEX_FORMAT_D24_UNORM_S8_UINT;
        inline constexpr TextureFormat R24_UNORM_X8_TYPELESS      = Diligent::TEX_FORMAT_R24_UNORM_X8_TYPELESS;
        inline constexpr TextureFormat X24_TYPELESS_G8_UINT       = Diligent::TEX_FORMAT_X24_TYPELESS_G8_UINT;
        inline constexpr TextureFormat RG8_TYPELESS               = Diligent::TEX_FORMAT_RG8_TYPELESS;
        inline constexpr TextureFormat RG8_UNORM                  = Diligent::TEX_FORMAT_RG8_UNORM;
        inline constexpr TextureFormat RG8_UINT                   = Diligent::TEX_FORMAT_RG8_UINT;
        inline constexpr TextureFormat RG8_SNORM                  = Diligent::TEX_FORMAT_RG8_SNORM;
        inline constexpr TextureFormat RG8_SINT                   = Diligent::TEX_FORMAT_RG8_SINT;
        inline constexpr TextureFormat R16_TYPELESS               = Diligent::TEX_FORMAT_R16_TYPELESS;
        inline constexpr TextureFormat R16_FLOAT                  = Diligent::TEX_FORMAT_R16_FLOAT;
        inline constexpr TextureFormat D16_UNORM                  = Diligent::TEX_FORMAT_D16_UNORM;
        inline constexpr TextureFormat R16_UNORM                  = Diligent::TEX_FORMAT_R16_UNORM;
        inline constexpr TextureFormat R16_UINT                   = Diligent::TEX_FORMAT_R16_UINT;
        inline constexpr TextureFormat R16_SNORM                  = Diligent::TEX_FORMAT_R16_SNORM;
        inline constexpr TextureFormat R16_SINT                   = Diligent::TEX_FORMAT_R16_SINT;
        inline constexpr TextureFormat R8_TYPELESS                = Diligent::TEX_FORMAT_R8_TYPELESS;
        inline constexpr TextureFormat R8_UNORM                   = Diligent::TEX_FORMAT_R8_UNORM;
        inline constexpr TextureFormat R8_UINT                    = Diligent::TEX_FORMAT_R8_UINT;
        inline constexpr TextureFormat R8_SNORM                   = Diligent::TEX_FORMAT_R8_SNORM;
        inline constexpr TextureFormat R8_SINT                    = Diligent::TEX_FORMAT_R8_SINT;
        inline constexpr TextureFormat A8_UNORM                   = Diligent::TEX_FORMAT_A8_UNORM;
        inline constexpr TextureFormat R1_UNORM                   = Diligent::TEX_FORMAT_R1_UNORM;
        inline constexpr TextureFormat RGB9E5_SHAREDEXP           = Diligent::TEX_FORMAT_RGB9E5_SHAREDEXP;
        inline constexpr TextureFormat RG8_B8G8_UNORM             = Diligent::TEX_FORMAT_RG8_B8G8_UNORM;
        inline constexpr TextureFormat G8R8_G8B8_UNORM            = Diligent::TEX_FORMAT_G8R8_G8B8_UNORM;
        inline constexpr TextureFormat BC1_TYPELESS               = Diligent::TEX_FORMAT_BC1_TYPELESS;
        inline constexpr TextureFormat BC1_UNORM                  = Diligent::TEX_FORMAT_BC1_UNORM;
        inline constexpr TextureFormat BC1_UNORM_SRGB             = Diligent::TEX_FORMAT_BC1_UNORM_SRGB;
        inline constexpr TextureFormat BC2_TYPELESS               = Diligent::TEX_FORMAT_BC2_TYPELESS;
        inline constexpr TextureFormat BC2_UNORM                  = Diligent::TEX_FORMAT_BC2_UNORM;
        inline constexpr TextureFormat BC2_UNORM_SRGB             = Diligent::TEX_FORMAT_BC2_UNORM_SRGB;
        inline constexpr TextureFormat BC3_TYPELESS               = Diligent::TEX_FORMAT_BC3_TYPELESS;
        inline constexpr TextureFormat BC3_UNORM                  = Diligent::TEX_FORMAT_BC3_UNORM;
        inline constexpr TextureFormat BC3_UNORM_SRGB             = Diligent::TEX_FORMAT_BC3_UNORM_SRGB;
        inline constexpr TextureFormat BC4_TYPELESS               = Diligent::TEX_FORMAT_BC4_TYPELESS;
        inline constexpr TextureFormat BC4_UNORM                  = Diligent::TEX_FORMAT_BC4_UNORM;
        inline constexpr TextureFormat BC4_SNORM                  = Diligent::TEX_FORMAT_BC4_SNORM;
        inline constexpr TextureFormat BC5_TYPELESS               = Diligent::TEX_FORMAT_BC5_TYPELESS;
        inline constexpr TextureFormat BC5_UNORM                  = Diligent::TEX_FORMAT_BC5_UNORM;
        inline constexpr TextureFormat BC5_SNORM                  = Diligent::TEX_FORMAT_BC5_SNORM;
        inline constexpr TextureFormat B5G6R5_UNORM               = Diligent::TEX_FORMAT_B5G6R5_UNORM;
        inline constexpr TextureFormat B5G5R5A1_UNORM             = Diligent::TEX_FORMAT_B5G5R5A1_UNORM;
        inline constexpr TextureFormat BGRA8_UNORM                = Diligent::TEX_FORMAT_BGRA8_UNORM;
        inline constexpr TextureFormat BGRX8_UNORM                = Diligent::TEX_FORMAT_BGRX8_UNORM;
        inline constexpr TextureFormat R10G10B10_XR_BIAS_A2_UNORM = Diligent::TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
        inline constexpr TextureFormat BGRA8_TYPELESS             = Diligent::TEX_FORMAT_BGRA8_TYPELESS;
        inline constexpr TextureFormat BGRA8_UNORM_SRGB           = Diligent::TEX_FORMAT_BGRA8_UNORM_SRGB;
        inline constexpr TextureFormat BGRX8_TYPELESS             = Diligent::TEX_FORMAT_BGRX8_TYPELESS;
        inline constexpr TextureFormat BGRX8_UNORM_SRGB           = Diligent::TEX_FORMAT_BGRX8_UNORM_SRGB;
        inline constexpr TextureFormat BC6H_TYPELESS              = Diligent::TEX_FORMAT_BC6H_TYPELESS;
        inline constexpr TextureFormat BC6H_UF16                  = Diligent::TEX_FORMAT_BC6H_UF16;
        inline constexpr TextureFormat BC6H_SF16                  = Diligent::TEX_FORMAT_BC6H_SF16;
        inline constexpr TextureFormat BC7_TYPELESS               = Diligent::TEX_FORMAT_BC7_TYPELESS;
        inline constexpr TextureFormat BC7_UNORM                  = Diligent::TEX_FORMAT_BC7_UNORM;
        inline constexpr TextureFormat BC7_UNORM_SRGB             = Diligent::TEX_FORMAT_BC7_UNORM_SRGB;
        inline constexpr TextureFormat ETC2_RGB8_UNORM            = Diligent::TEX_FORMAT_ETC2_RGB8_UNORM;
        inline constexpr TextureFormat ETC2_RGB8_UNORM_SRGB       = Diligent::TEX_FORMAT_ETC2_RGB8_UNORM_SRGB;
        inline constexpr TextureFormat ETC2_RGB8A1_UNORM          = Diligent::TEX_FORMAT_ETC2_RGB8A1_UNORM;
        inline constexpr TextureFormat ETC2_RGB8A1_UNORM_SRGB     = Diligent::TEX_FORMAT_ETC2_RGB8A1_UNORM_SRGB;
        inline constexpr TextureFormat ETC2_RGBA8_UNORM           = Diligent::TEX_FORMAT_ETC2_RGBA8_UNORM;
        inline constexpr TextureFormat ETC2_RGBA8_UNORM_SRGB      = Diligent::TEX_FORMAT_ETC2_RGBA8_UNORM_SRGB;
        inline constexpr TextureFormat NUM_FORMATS                = Diligent::TEX_FORMAT_NUM_FORMATS;
    }

    using FilterType = Diligent::FILTER_TYPE;
    namespace FilterTypes {
        inline constexpr FilterType UNKNOWN                = Diligent::FILTER_TYPE_UNKNOWN;
        inline constexpr FilterType POINT                  = Diligent::FILTER_TYPE_POINT;
        inline constexpr FilterType LINEAR                 = Diligent::FILTER_TYPE_LINEAR;
        inline constexpr FilterType ANISOTROPIC            = Diligent::FILTER_TYPE_ANISOTROPIC;
        inline constexpr FilterType COMPARISON_POINT       = Diligent::FILTER_TYPE_COMPARISON_POINT;
        inline constexpr FilterType COMPARISON_LINEAR      = Diligent::FILTER_TYPE_COMPARISON_LINEAR;
        inline constexpr FilterType COMPARISON_ANISOTROPIC = Diligent::FILTER_TYPE_COMPARISON_ANISOTROPIC;
        inline constexpr FilterType MINIMUM_POINT          = Diligent::FILTER_TYPE_MINIMUM_POINT;
        inline constexpr FilterType MINIMUM_LINEAR         = Diligent::FILTER_TYPE_MINIMUM_LINEAR;
        inline constexpr FilterType MINIMUM_ANISOTROPIC    = Diligent::FILTER_TYPE_MINIMUM_ANISOTROPIC;
        inline constexpr FilterType MAXIMUM_POINT          = Diligent::FILTER_TYPE_MAXIMUM_POINT;
        inline constexpr FilterType MAXIMUM_LINEAR         = Diligent::FILTER_TYPE_MAXIMUM_LINEAR;
        inline constexpr FilterType MAXIMUM_ANISOTROPIC    = Diligent::FILTER_TYPE_MAXIMUM_ANISOTROPIC;
        inline constexpr FilterType NUM_FILTERS            = Diligent::FILTER_TYPE_NUM_FILTERS;
    }

    using TextureAddressMode = Diligent::TEXTURE_ADDRESS_MODE;
    namespace TextureAddressModes {
        inline constexpr TextureAddressMode UNKNOWN     = Diligent::TEXTURE_ADDRESS_UNKNOWN;
        inline constexpr TextureAddressMode WRAP        = Diligent::TEXTURE_ADDRESS_WRAP;
        inline constexpr TextureAddressMode MIRROR      = Diligent::TEXTURE_ADDRESS_MIRROR;
        inline constexpr TextureAddressMode CLAMP       = Diligent::TEXTURE_ADDRESS_CLAMP;
        inline constexpr TextureAddressMode BORDER      = Diligent::TEXTURE_ADDRESS_BORDER;
        inline constexpr TextureAddressMode MIRROR_ONCE = Diligent::TEXTURE_ADDRESS_MIRROR_ONCE;
        inline constexpr TextureAddressMode NUM_MODES   = Diligent::TEXTURE_ADDRESS_NUM_MODES;
    }

    using ComparisonFunction = Diligent::COMPARISON_FUNCTION;
    namespace ComparisonFunctions {
        inline constexpr ComparisonFunction UNKNOWN       = Diligent::COMPARISON_FUNC_UNKNOWN;
        inline constexpr ComparisonFunction NEVER         = Diligent::COMPARISON_FUNC_NEVER;
        inline constexpr ComparisonFunction LESS          = Diligent::COMPARISON_FUNC_LESS;
        inline constexpr ComparisonFunction EQUAL         = Diligent::COMPARISON_FUNC_EQUAL;
        inline constexpr ComparisonFunction LESS_EQUAL    = Diligent::COMPARISON_FUNC_LESS_EQUAL;
        inline constexpr ComparisonFunction GREATER       = Diligent::COMPARISON_FUNC_GREATER;
        inline constexpr ComparisonFunction NOT_EQUAL     = Diligent::COMPARISON_FUNC_NOT_EQUAL;
        inline constexpr ComparisonFunction GREATER_EQUAL = Diligent::COMPARISON_FUNC_GREATER_EQUAL;
        inline constexpr ComparisonFunction ALWAYS        = Diligent::COMPARISON_FUNC_ALWAYS;
        inline constexpr ComparisonFunction NUM_FUNCTIONS = Diligent::COMPARISON_FUNC_NUM_FUNCTIONS;
    }

    using PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY;
    namespace PrimitiveTopologys {
        inline constexpr PrimitiveTopology UNDEFINED                           = Diligent::PRIMITIVE_TOPOLOGY_UNDEFINED;
        inline constexpr PrimitiveTopology TRIANGLE_LIST                       = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inline constexpr PrimitiveTopology TRIANGLE_STRIP                      = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        inline constexpr PrimitiveTopology POINT_LIST                          = Diligent::PRIMITIVE_TOPOLOGY_POINT_LIST;
        inline constexpr PrimitiveTopology LINE_LIST                           = Diligent::PRIMITIVE_TOPOLOGY_LINE_LIST;
        inline constexpr PrimitiveTopology LINE_STRIP                          = Diligent::PRIMITIVE_TOPOLOGY_LINE_STRIP;
        inline constexpr PrimitiveTopology TRIANGLE_LIST_ADJ                   = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJ;
        inline constexpr PrimitiveTopology TRIANGLE_STRIP_ADJ                  = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_ADJ;
        inline constexpr PrimitiveTopology LINE_LIST_ADJ                       = Diligent::PRIMITIVE_TOPOLOGY_LINE_LIST_ADJ;
        inline constexpr PrimitiveTopology LINE_STRIP_ADJ                      = Diligent::PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJ;
        inline constexpr PrimitiveTopology TOPOLOGY_1_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_2_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_3_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_4_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_5_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_6_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_7_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_8_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_9_CONTROL_POINT_PATCHLIST  = Diligent::PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_10_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_11_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_12_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_13_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_14_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_15_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_16_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_17_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_18_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_19_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_20_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_21_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_22_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_23_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_24_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_25_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_26_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_27_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_28_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_29_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_30_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_31_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology TOPOLOGY_32_CONTROL_POINT_PATCHLIST = Diligent::PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
        inline constexpr PrimitiveTopology NUM_TOPOLOGIES                      = Diligent::PRIMITIVE_TOPOLOGY_NUM_TOPOLOGIES;
    }

    using MemoryPropertie = Diligent::MEMORY_PROPERTIES;
    namespace MemoryProperties {
        inline constexpr MemoryPropertie PROPERTY_UNKNOWN       = Diligent::MEMORY_PROPERTY_UNKNOWN;
        inline constexpr MemoryPropertie PROPERTY_HOST_COHERENT = Diligent::MEMORY_PROPERTY_HOST_COHERENT;
    }

    using AdapterType = Diligent::ADAPTER_TYPE;
    namespace AdapterTypes {
        inline constexpr AdapterType UNKNOWN    = Diligent::ADAPTER_TYPE_UNKNOWN;
        inline constexpr AdapterType SOFTWARE   = Diligent::ADAPTER_TYPE_SOFTWARE;
        inline constexpr AdapterType INTEGRATED = Diligent::ADAPTER_TYPE_INTEGRATED;
        inline constexpr AdapterType DISCRETE   = Diligent::ADAPTER_TYPE_DISCRETE;
        inline constexpr AdapterType COUNT      = Diligent::ADAPTER_TYPE_COUNT;
    }

    using SwapChainUsageFlag = Diligent::SWAP_CHAIN_USAGE_FLAGS;
    namespace SwapChainUsageFlags {
        inline constexpr SwapChainUsageFlag NONE             = Diligent::SWAP_CHAIN_USAGE_NONE;
        inline constexpr SwapChainUsageFlag RENDER_TARGET    = Diligent::SWAP_CHAIN_USAGE_RENDER_TARGET;
        inline constexpr SwapChainUsageFlag SHADER_RESOURCE  = Diligent::SWAP_CHAIN_USAGE_SHADER_RESOURCE;
        inline constexpr SwapChainUsageFlag INPUT_ATTACHMENT = Diligent::SWAP_CHAIN_USAGE_INPUT_ATTACHMENT;
        inline constexpr SwapChainUsageFlag COPY_SOURCE      = Diligent::SWAP_CHAIN_USAGE_COPY_SOURCE;
        inline constexpr SwapChainUsageFlag LAST             = Diligent::SWAP_CHAIN_USAGE_LAST;
    }

    using SurfaceTransform = Diligent::SURFACE_TRANSFORM;
    namespace SurfaceTransforms {
        inline constexpr SurfaceTransform OPTIMAL                      = Diligent::SURFACE_TRANSFORM_OPTIMAL;
        inline constexpr SurfaceTransform IDENTITY                     = Diligent::SURFACE_TRANSFORM_IDENTITY;
        inline constexpr SurfaceTransform ROTATE_90                    = Diligent::SURFACE_TRANSFORM_ROTATE_90;
        inline constexpr SurfaceTransform ROTATE_180                   = Diligent::SURFACE_TRANSFORM_ROTATE_180;
        inline constexpr SurfaceTransform ROTATE_270                   = Diligent::SURFACE_TRANSFORM_ROTATE_270;
        inline constexpr SurfaceTransform HORIZONTAL_MIRROR            = Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR;
        inline constexpr SurfaceTransform HORIZONTAL_MIRROR_ROTATE_90  = Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90;
        inline constexpr SurfaceTransform HORIZONTAL_MIRROR_ROTATE_180 = Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180;
        inline constexpr SurfaceTransform HORIZONTAL_MIRROR_ROTATE_270 = Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270;
    }

    using DeviceFeatureState = Diligent::DEVICE_FEATURE_STATE;
    namespace DeviceFeatureStates {
        inline constexpr DeviceFeatureState DISABLED = Diligent::DEVICE_FEATURE_STATE_DISABLED;
        inline constexpr DeviceFeatureState ENABLED  = Diligent::DEVICE_FEATURE_STATE_ENABLED;
        inline constexpr DeviceFeatureState OPTIONAL = Diligent::DEVICE_FEATURE_STATE_OPTIONAL;
    }

    using AdapterVendor = Diligent::ADAPTER_VENDOR;
    namespace AdapterVendors {
        inline constexpr AdapterVendor UNKNOWN  = Diligent::ADAPTER_VENDOR_UNKNOWN;
        inline constexpr AdapterVendor NVIDIA   = Diligent::ADAPTER_VENDOR_NVIDIA;
        inline constexpr AdapterVendor AMD      = Diligent::ADAPTER_VENDOR_AMD;
        inline constexpr AdapterVendor INTEL    = Diligent::ADAPTER_VENDOR_INTEL;
        inline constexpr AdapterVendor ARM      = Diligent::ADAPTER_VENDOR_ARM;
        inline constexpr AdapterVendor QUALCOMM = Diligent::ADAPTER_VENDOR_QUALCOMM;
        inline constexpr AdapterVendor IMGTECH  = Diligent::ADAPTER_VENDOR_IMGTECH;
        inline constexpr AdapterVendor MSFT     = Diligent::ADAPTER_VENDOR_MSFT;
        inline constexpr AdapterVendor APPLE    = Diligent::ADAPTER_VENDOR_APPLE;
        inline constexpr AdapterVendor MESA     = Diligent::ADAPTER_VENDOR_MESA;
        inline constexpr AdapterVendor BROADCOM = Diligent::ADAPTER_VENDOR_BROADCOM;
        inline constexpr AdapterVendor LAST     = Diligent::ADAPTER_VENDOR_LAST;
    }

    using WaveFeature = Diligent::WAVE_FEATURE;
    namespace WaveFeatures {
        inline constexpr WaveFeature UNKNOWN          = Diligent::WAVE_FEATURE_UNKNOWN;
        inline constexpr WaveFeature BASIC            = Diligent::WAVE_FEATURE_BASIC;
        inline constexpr WaveFeature VOTE             = Diligent::WAVE_FEATURE_VOTE;
        inline constexpr WaveFeature ARITHMETIC       = Diligent::WAVE_FEATURE_ARITHMETIC;
        inline constexpr WaveFeature BALLOUT          = Diligent::WAVE_FEATURE_BALLOUT;
        inline constexpr WaveFeature SHUFFLE          = Diligent::WAVE_FEATURE_SHUFFLE;
        inline constexpr WaveFeature SHUFFLE_RELATIVE = Diligent::WAVE_FEATURE_SHUFFLE_RELATIVE;
        inline constexpr WaveFeature CLUSTERED        = Diligent::WAVE_FEATURE_CLUSTERED;
        inline constexpr WaveFeature QUAD             = Diligent::WAVE_FEATURE_QUAD;
        inline constexpr WaveFeature LAST             = Diligent::WAVE_FEATURE_LAST;
    }

    using ValidationLevel = Diligent::VALIDATION_LEVEL;
    namespace ValidationLevels {
        inline constexpr ValidationLevel DISABLED = Diligent::VALIDATION_LEVEL_DISABLED;
        inline constexpr ValidationLevel LEVEL_1  = Diligent::VALIDATION_LEVEL_1;
        inline constexpr ValidationLevel LEVEL_2  = Diligent::VALIDATION_LEVEL_2;
    }

    using RayTracingCapFlag = Diligent::RAY_TRACING_CAP_FLAGS;
    namespace RayTracingCapFlags {
        inline constexpr RayTracingCapFlag NONE                 = Diligent::RAY_TRACING_CAP_FLAG_NONE;
        inline constexpr RayTracingCapFlag STANDALONE_SHADERS   = Diligent::RAY_TRACING_CAP_FLAG_STANDALONE_SHADERS;
        inline constexpr RayTracingCapFlag INLINE_RAY_TRACING   = Diligent::RAY_TRACING_CAP_FLAG_INLINE_RAY_TRACING;
        inline constexpr RayTracingCapFlag INDIRECT_RAY_TRACING = Diligent::RAY_TRACING_CAP_FLAG_INDIRECT_RAY_TRACING;
    }

    using ValidationFlag = Diligent::VALIDATION_FLAGS;
    namespace ValidationFlags {
        inline constexpr ValidationFlag NONE                     = Diligent::VALIDATION_FLAG_NONE;
        inline constexpr ValidationFlag CHECK_SHADER_BUFFER_SIZE = Diligent::VALIDATION_FLAG_CHECK_SHADER_BUFFER_SIZE;
    }

    using CommandQueueType = Diligent::COMMAND_QUEUE_TYPE;
    namespace CommandQueueTypes {
        inline constexpr CommandQueueType UNKNOWN        = Diligent::COMMAND_QUEUE_TYPE_UNKNOWN;
        inline constexpr CommandQueueType TRANSFER       = Diligent::COMMAND_QUEUE_TYPE_TRANSFER;
        inline constexpr CommandQueueType COMPUTE        = Diligent::COMMAND_QUEUE_TYPE_COMPUTE;
        inline constexpr CommandQueueType GRAPHICS       = Diligent::COMMAND_QUEUE_TYPE_GRAPHICS;
        inline constexpr CommandQueueType PRIMARY_MASK   = Diligent::COMMAND_QUEUE_TYPE_PRIMARY_MASK;
        inline constexpr CommandQueueType SPARSE_BINDING = Diligent::COMMAND_QUEUE_TYPE_SPARSE_BINDING;
        inline constexpr CommandQueueType MAX_BIT        = Diligent::COMMAND_QUEUE_TYPE_MAX_BIT;
    }

    using QueuePriority = Diligent::QUEUE_PRIORITY;
    namespace QueuePrioritys {
        inline constexpr QueuePriority UNKNOWN  = Diligent::QUEUE_PRIORITY_UNKNOWN;
        inline constexpr QueuePriority LOW      = Diligent::QUEUE_PRIORITY_LOW;
        inline constexpr QueuePriority MEDIUM   = Diligent::QUEUE_PRIORITY_MEDIUM;
        inline constexpr QueuePriority HIGH     = Diligent::QUEUE_PRIORITY_HIGH;
        inline constexpr QueuePriority REALTIME = Diligent::QUEUE_PRIORITY_REALTIME;
        inline constexpr QueuePriority LAST     = Diligent::QUEUE_PRIORITY_LAST;
    }

    using ShadingRateCombiner = Diligent::SHADING_RATE_COMBINER;
    namespace ShadingRateCombiners {
        inline constexpr ShadingRateCombiner PASSTHROUGH = Diligent::SHADING_RATE_COMBINER_PASSTHROUGH;
        inline constexpr ShadingRateCombiner OVERRIDE    = Diligent::SHADING_RATE_COMBINER_OVERRIDE;
        inline constexpr ShadingRateCombiner MIN         = Diligent::SHADING_RATE_COMBINER_MIN;
        inline constexpr ShadingRateCombiner MAX         = Diligent::SHADING_RATE_COMBINER_MAX;
        inline constexpr ShadingRateCombiner SUM         = Diligent::SHADING_RATE_COMBINER_SUM;
        inline constexpr ShadingRateCombiner MUL         = Diligent::SHADING_RATE_COMBINER_MUL;
        inline constexpr ShadingRateCombiner LAST        = Diligent::SHADING_RATE_COMBINER_LAST;
    }

    using ShadingRateFormat = Diligent::SHADING_RATE_FORMAT;
    namespace ShadingRateFormats {
        inline constexpr ShadingRateFormat UNKNOWN      = Diligent::SHADING_RATE_FORMAT_UNKNOWN;
        inline constexpr ShadingRateFormat PALETTE      = Diligent::SHADING_RATE_FORMAT_PALETTE;
        inline constexpr ShadingRateFormat UNORM8       = Diligent::SHADING_RATE_FORMAT_UNORM8;
        inline constexpr ShadingRateFormat COL_ROW_FP32 = Diligent::SHADING_RATE_FORMAT_COL_ROW_FP32;
    }

    using AxisShadingRate = Diligent::AXIS_SHADING_RATE;
    namespace AxisShadingRates {
        inline constexpr AxisShadingRate RATE_1X = Diligent::AXIS_SHADING_RATE_1X;
        inline constexpr AxisShadingRate RATE_2X = Diligent::AXIS_SHADING_RATE_2X;
        inline constexpr AxisShadingRate RATE_4X = Diligent::AXIS_SHADING_RATE_4X;
        inline constexpr AxisShadingRate MAX     = Diligent::AXIS_SHADING_RATE_MAX;
    }

    using ShadingRate = Diligent::SHADING_RATE;
    namespace ShadingRates {
        inline constexpr ShadingRate RATE_1X1 = Diligent::SHADING_RATE_1X1;
        inline constexpr ShadingRate RATE_1X2 = Diligent::SHADING_RATE_1X2;
        inline constexpr ShadingRate RATE_1X4 = Diligent::SHADING_RATE_1X4;
        inline constexpr ShadingRate RATE_2X1 = Diligent::SHADING_RATE_2X1;
        inline constexpr ShadingRate RATE_2X2 = Diligent::SHADING_RATE_2X2;
        inline constexpr ShadingRate RATE_2X4 = Diligent::SHADING_RATE_2X4;
        inline constexpr ShadingRate RATE_4X1 = Diligent::SHADING_RATE_4X1;
        inline constexpr ShadingRate RATE_4X2 = Diligent::SHADING_RATE_4X2;
        inline constexpr ShadingRate RATE_4X4 = Diligent::SHADING_RATE_4X4;
        inline constexpr ShadingRate MAX      = Diligent::SHADING_RATE_MAX;
    }

    using SampleCount = Diligent::SAMPLE_COUNT;
    namespace SampleCounts {
        inline constexpr SampleCount NONE     = Diligent::SAMPLE_COUNT_NONE;
        inline constexpr SampleCount COUNT_1  = Diligent::SAMPLE_COUNT_1;
        inline constexpr SampleCount COUNT_2  = Diligent::SAMPLE_COUNT_2;
        inline constexpr SampleCount COUNT_4  = Diligent::SAMPLE_COUNT_4;
        inline constexpr SampleCount COUNT_8  = Diligent::SAMPLE_COUNT_8;
        inline constexpr SampleCount COUNT_16 = Diligent::SAMPLE_COUNT_16;
        inline constexpr SampleCount COUNT_32 = Diligent::SAMPLE_COUNT_32;
        inline constexpr SampleCount COUNT_64 = Diligent::SAMPLE_COUNT_64;
        inline constexpr SampleCount MAX      = Diligent::SAMPLE_COUNT_MAX;
        inline constexpr SampleCount ALL      = Diligent::SAMPLE_COUNT_ALL;
    }

    using ShadingRateCapFlag = Diligent::SHADING_RATE_CAP_FLAGS;
    namespace ShadingRateCapFlags {
        inline constexpr ShadingRateCapFlag NONE                       = Diligent::SHADING_RATE_CAP_FLAG_NONE;
        inline constexpr ShadingRateCapFlag PER_DRAW                   = Diligent::SHADING_RATE_CAP_FLAG_PER_DRAW;
        inline constexpr ShadingRateCapFlag PER_PRIMITIVE              = Diligent::SHADING_RATE_CAP_FLAG_PER_PRIMITIVE;
        inline constexpr ShadingRateCapFlag TEXTURE_BASED              = Diligent::SHADING_RATE_CAP_FLAG_TEXTURE_BASED;
        inline constexpr ShadingRateCapFlag SAMPLE_MASK                = Diligent::SHADING_RATE_CAP_FLAG_SAMPLE_MASK;
        inline constexpr ShadingRateCapFlag SHADER_SAMPLE_MASK         = Diligent::SHADING_RATE_CAP_FLAG_SHADER_SAMPLE_MASK;
        inline constexpr ShadingRateCapFlag SHADER_DEPTH_STENCIL_WRITE = Diligent::SHADING_RATE_CAP_FLAG_SHADER_DEPTH_STENCIL_WRITE;
        inline constexpr ShadingRateCapFlag
            PER_PRIMITIVE_WITH_MULTIPLE_VIEWPORTS = Diligent::SHADING_RATE_CAP_FLAG_PER_PRIMITIVE_WITH_MULTIPLE_VIEWPORTS;
        inline constexpr ShadingRateCapFlag SAME_TEXTURE_FOR_WHOLE_RENDERPASS = Diligent::SHADING_RATE_CAP_FLAG_SAME_TEXTURE_FOR_WHOLE_RENDERPASS;
        inline constexpr ShadingRateCapFlag TEXTURE_ARRAY                = Diligent::SHADING_RATE_CAP_FLAG_TEXTURE_ARRAY;
        inline constexpr ShadingRateCapFlag SHADING_RATE_SHADER_INPUT    = Diligent::SHADING_RATE_CAP_FLAG_SHADING_RATE_SHADER_INPUT;
        inline constexpr ShadingRateCapFlag ADDITIONAL_INVOCATIONS       = Diligent::SHADING_RATE_CAP_FLAG_ADDITIONAL_INVOCATIONS;
        inline constexpr ShadingRateCapFlag NON_SUBSAMPLED_RENDER_TARGET = Diligent::SHADING_RATE_CAP_FLAG_NON_SUBSAMPLED_RENDER_TARGET;
        inline constexpr ShadingRateCapFlag SUBSAMPLED_RENDER_TARGET     = Diligent::SHADING_RATE_CAP_FLAG_SUBSAMPLED_RENDER_TARGET;
    }

    using ShadingRateTextureAcces = Diligent::SHADING_RATE_TEXTURE_ACCESS;
    namespace ShadingRateTextureAccess {
        inline constexpr ShadingRateTextureAcces UNKNOWN    = Diligent::SHADING_RATE_TEXTURE_ACCESS_UNKNOWN;
        inline constexpr ShadingRateTextureAcces ON_GPU     = Diligent::SHADING_RATE_TEXTURE_ACCESS_ON_GPU;
        inline constexpr ShadingRateTextureAcces ON_SUBMIT  = Diligent::SHADING_RATE_TEXTURE_ACCESS_ON_SUBMIT;
        inline constexpr ShadingRateTextureAcces ON_SET_RTV = Diligent::SHADING_RATE_TEXTURE_ACCESS_ON_SET_RTV;
    }

    using DrawCommandCapFlag = Diligent::DRAW_COMMAND_CAP_FLAGS;
    namespace DrawCommandCapFlags {
        inline constexpr DrawCommandCapFlag NONE                         = Diligent::DRAW_COMMAND_CAP_FLAG_NONE;
        inline constexpr DrawCommandCapFlag BASE_VERTEX                  = Diligent::DRAW_COMMAND_CAP_FLAG_BASE_VERTEX;
        inline constexpr DrawCommandCapFlag DRAW_INDIRECT                = Diligent::DRAW_COMMAND_CAP_FLAG_DRAW_INDIRECT;
        inline constexpr DrawCommandCapFlag DRAW_INDIRECT_FIRST_INSTANCE = Diligent::DRAW_COMMAND_CAP_FLAG_DRAW_INDIRECT_FIRST_INSTANCE;
        inline constexpr DrawCommandCapFlag NATIVE_MULTI_DRAW_INDIRECT   = Diligent::DRAW_COMMAND_CAP_FLAG_NATIVE_MULTI_DRAW_INDIRECT;
        inline constexpr DrawCommandCapFlag DRAW_INDIRECT_COUNTER_BUFFER = Diligent::DRAW_COMMAND_CAP_FLAG_DRAW_INDIRECT_COUNTER_BUFFER;
    }

    using SparseResourceCapFlag = Diligent::SPARSE_RESOURCE_CAP_FLAGS;
    namespace SparseResourceCapFlags {
        inline constexpr SparseResourceCapFlag NONE                        = Diligent::SPARSE_RESOURCE_CAP_FLAG_NONE;
        inline constexpr SparseResourceCapFlag SHADER_RESOURCE_RESIDENCY   = Diligent::SPARSE_RESOURCE_CAP_FLAG_SHADER_RESOURCE_RESIDENCY;
        inline constexpr SparseResourceCapFlag BUFFER                      = Diligent::SPARSE_RESOURCE_CAP_FLAG_BUFFER;
        inline constexpr SparseResourceCapFlag TEXTURE_2D                  = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_2D;
        inline constexpr SparseResourceCapFlag TEXTURE_3D                  = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_3D;
        inline constexpr SparseResourceCapFlag TEXTURE_2_SAMPLES           = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_2_SAMPLES;
        inline constexpr SparseResourceCapFlag TEXTURE_4_SAMPLES           = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_4_SAMPLES;
        inline constexpr SparseResourceCapFlag TEXTURE_8_SAMPLES           = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_8_SAMPLES;
        inline constexpr SparseResourceCapFlag TEXTURE_16_SAMPLES          = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_16_SAMPLES;
        inline constexpr SparseResourceCapFlag ALIASED                     = Diligent::SPARSE_RESOURCE_CAP_FLAG_ALIASED;
        inline constexpr SparseResourceCapFlag STANDARD_2D_TILE_SHAPE      = Diligent::SPARSE_RESOURCE_CAP_FLAG_STANDARD_2D_TILE_SHAPE;
        inline constexpr SparseResourceCapFlag STANDARD_2DMS_TILE_SHAPE    = Diligent::SPARSE_RESOURCE_CAP_FLAG_STANDARD_2DMS_TILE_SHAPE;
        inline constexpr SparseResourceCapFlag STANDARD_3D_TILE_SHAPE      = Diligent::SPARSE_RESOURCE_CAP_FLAG_STANDARD_3D_TILE_SHAPE;
        inline constexpr SparseResourceCapFlag ALIGNED_MIP_SIZE            = Diligent::SPARSE_RESOURCE_CAP_FLAG_ALIGNED_MIP_SIZE;
        inline constexpr SparseResourceCapFlag NON_RESIDENT_STRICT         = Diligent::SPARSE_RESOURCE_CAP_FLAG_NON_RESIDENT_STRICT;
        inline constexpr SparseResourceCapFlag TEXTURE_2D_ARRAY_MIP_TAIL   = Diligent::SPARSE_RESOURCE_CAP_FLAG_TEXTURE_2D_ARRAY_MIP_TAIL;
        inline constexpr SparseResourceCapFlag BUFFER_STANDARD_BLOCK       = Diligent::SPARSE_RESOURCE_CAP_FLAG_BUFFER_STANDARD_BLOCK;
        inline constexpr SparseResourceCapFlag NON_RESIDENT_SAFE           = Diligent::SPARSE_RESOURCE_CAP_FLAG_NON_RESIDENT_SAFE;
        inline constexpr SparseResourceCapFlag MIXED_RESOURCE_TYPE_SUPPORT = Diligent::SPARSE_RESOURCE_CAP_FLAG_MIXED_RESOURCE_TYPE_SUPPORT;
    }

    using D3d11ValidationFlag = Diligent::D3D11_VALIDATION_FLAGS;
    namespace D3d11ValidationFlags {
        inline constexpr D3d11ValidationFlag NONE = Diligent::D3D11_VALIDATION_FLAG_NONE;
        inline constexpr D3d11ValidationFlag VERIFY_COMMITTED_RESOURCE_RELEVANCE = Diligent::D3D11_VALIDATION_FLAG_VERIFY_COMMITTED_RESOURCE_RELEVANCE;
    }

    using D3d12ValidationFlag = Diligent::D3D12_VALIDATION_FLAGS;
    namespace D3d12ValidationFlags {
        inline constexpr D3d12ValidationFlag NONE                        = Diligent::D3D12_VALIDATION_FLAG_NONE;
        inline constexpr D3d12ValidationFlag BREAK_ON_ERROR              = Diligent::D3D12_VALIDATION_FLAG_BREAK_ON_ERROR;
        inline constexpr D3d12ValidationFlag BREAK_ON_CORRUPTION         = Diligent::D3D12_VALIDATION_FLAG_BREAK_ON_CORRUPTION;
        inline constexpr D3d12ValidationFlag ENABLE_GPU_BASED_VALIDATION = Diligent::D3D12_VALIDATION_FLAG_ENABLE_GPU_BASED_VALIDATION;
    }

    using ComponentType = Diligent::COMPONENT_TYPE;
    namespace ComponentTypes {
        inline constexpr ComponentType UNDEFINED     = Diligent::COMPONENT_TYPE_UNDEFINED;
        inline constexpr ComponentType FLOAT         = Diligent::COMPONENT_TYPE_FLOAT;
        inline constexpr ComponentType SNORM         = Diligent::COMPONENT_TYPE_SNORM;
        inline constexpr ComponentType UNORM         = Diligent::COMPONENT_TYPE_UNORM;
        inline constexpr ComponentType UNORM_SRGB    = Diligent::COMPONENT_TYPE_UNORM_SRGB;
        inline constexpr ComponentType SINT          = Diligent::COMPONENT_TYPE_SINT;
        inline constexpr ComponentType UINT          = Diligent::COMPONENT_TYPE_UINT;
        inline constexpr ComponentType DEPTH         = Diligent::COMPONENT_TYPE_DEPTH;
        inline constexpr ComponentType DEPTH_STENCIL = Diligent::COMPONENT_TYPE_DEPTH_STENCIL;
        inline constexpr ComponentType COMPOUND      = Diligent::COMPONENT_TYPE_COMPOUND;
        inline constexpr ComponentType COMPRESSED    = Diligent::COMPONENT_TYPE_COMPRESSED;
    }

    using ResourceDimensionSupport = Diligent::RESOURCE_DIMENSION_SUPPORT;
    namespace ResourceDimensionSupports {
        inline constexpr ResourceDimensionSupport NONE           = Diligent::RESOURCE_DIMENSION_SUPPORT_NONE;
        inline constexpr ResourceDimensionSupport BUFFER         = Diligent::RESOURCE_DIMENSION_SUPPORT_BUFFER;
        inline constexpr ResourceDimensionSupport TEX_1D         = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_1D;
        inline constexpr ResourceDimensionSupport TEX_1D_ARRAY   = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_1D_ARRAY;
        inline constexpr ResourceDimensionSupport TEX_2D         = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_2D;
        inline constexpr ResourceDimensionSupport TEX_2D_ARRAY   = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_2D_ARRAY;
        inline constexpr ResourceDimensionSupport TEX_3D         = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_3D;
        inline constexpr ResourceDimensionSupport TEX_CUBE       = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_CUBE;
        inline constexpr ResourceDimensionSupport TEX_CUBE_ARRAY = Diligent::RESOURCE_DIMENSION_SUPPORT_TEX_CUBE_ARRAY;
    }

    using SparseTextureFlag = Diligent::SPARSE_TEXTURE_FLAGS;
    namespace SparseTextureFlags {
        inline constexpr SparseTextureFlag NONE                   = Diligent::SPARSE_TEXTURE_FLAG_NONE;
        inline constexpr SparseTextureFlag SINGLE_MIPTAIL         = Diligent::SPARSE_TEXTURE_FLAG_SINGLE_MIPTAIL;
        inline constexpr SparseTextureFlag ALIGNED_MIP_SIZE       = Diligent::SPARSE_TEXTURE_FLAG_ALIGNED_MIP_SIZE;
        inline constexpr SparseTextureFlag NONSTANDARD_BLOCK_SIZE = Diligent::SPARSE_TEXTURE_FLAG_NONSTANDARD_BLOCK_SIZE;
        inline constexpr SparseTextureFlag LAST                   = Diligent::SPARSE_TEXTURE_FLAG_LAST;
    }

    using PipelineStageFlag = Diligent::PIPELINE_STAGE_FLAGS;
    namespace PipelineStageFlags {
        inline constexpr PipelineStageFlag UNDEFINED                    = Diligent::PIPELINE_STAGE_FLAG_UNDEFINED;
        inline constexpr PipelineStageFlag TOP_OF_PIPE                  = Diligent::PIPELINE_STAGE_FLAG_TOP_OF_PIPE;
        inline constexpr PipelineStageFlag DRAW_INDIRECT                = Diligent::PIPELINE_STAGE_FLAG_DRAW_INDIRECT;
        inline constexpr PipelineStageFlag VERTEX_INPUT                 = Diligent::PIPELINE_STAGE_FLAG_VERTEX_INPUT;
        inline constexpr PipelineStageFlag VERTEX_SHADER                = Diligent::PIPELINE_STAGE_FLAG_VERTEX_SHADER;
        inline constexpr PipelineStageFlag HULL_SHADER                  = Diligent::PIPELINE_STAGE_FLAG_HULL_SHADER;
        inline constexpr PipelineStageFlag DOMAIN_SHADER                = Diligent::PIPELINE_STAGE_FLAG_DOMAIN_SHADER;
        inline constexpr PipelineStageFlag GEOMETRY_SHADER              = Diligent::PIPELINE_STAGE_FLAG_GEOMETRY_SHADER;
        inline constexpr PipelineStageFlag PIXEL_SHADER                 = Diligent::PIPELINE_STAGE_FLAG_PIXEL_SHADER;
        inline constexpr PipelineStageFlag EARLY_FRAGMENT_TESTS         = Diligent::PIPELINE_STAGE_FLAG_EARLY_FRAGMENT_TESTS;
        inline constexpr PipelineStageFlag LATE_FRAGMENT_TESTS          = Diligent::PIPELINE_STAGE_FLAG_LATE_FRAGMENT_TESTS;
        inline constexpr PipelineStageFlag RENDER_TARGET                = Diligent::PIPELINE_STAGE_FLAG_RENDER_TARGET;
        inline constexpr PipelineStageFlag COMPUTE_SHADER               = Diligent::PIPELINE_STAGE_FLAG_COMPUTE_SHADER;
        inline constexpr PipelineStageFlag TRANSFER                     = Diligent::PIPELINE_STAGE_FLAG_TRANSFER;
        inline constexpr PipelineStageFlag BOTTOM_OF_PIPE               = Diligent::PIPELINE_STAGE_FLAG_BOTTOM_OF_PIPE;
        inline constexpr PipelineStageFlag HOST                         = Diligent::PIPELINE_STAGE_FLAG_HOST;
        inline constexpr PipelineStageFlag CONDITIONAL_RENDERING        = Diligent::PIPELINE_STAGE_FLAG_CONDITIONAL_RENDERING;
        inline constexpr PipelineStageFlag SHADING_RATE_TEXTURE         = Diligent::PIPELINE_STAGE_FLAG_SHADING_RATE_TEXTURE;
        inline constexpr PipelineStageFlag RAY_TRACING_SHADER           = Diligent::PIPELINE_STAGE_FLAG_RAY_TRACING_SHADER;
        inline constexpr PipelineStageFlag ACCELERATION_STRUCTURE_BUILD = Diligent::PIPELINE_STAGE_FLAG_ACCELERATION_STRUCTURE_BUILD;
        inline constexpr PipelineStageFlag TASK_SHADER                  = Diligent::PIPELINE_STAGE_FLAG_TASK_SHADER;
        inline constexpr PipelineStageFlag MESH_SHADER                  = Diligent::PIPELINE_STAGE_FLAG_MESH_SHADER;
        inline constexpr PipelineStageFlag FRAGMENT_DENSITY_PROCESS     = Diligent::PIPELINE_STAGE_FLAG_FRAGMENT_DENSITY_PROCESS;
        inline constexpr PipelineStageFlag DEFAULT                      = Diligent::PIPELINE_STAGE_FLAG_DEFAULT;
    }

    using AccessFlag = Diligent::ACCESS_FLAGS;
    namespace AccessFlags {
        inline constexpr AccessFlag NONE                         = Diligent::ACCESS_FLAG_NONE;
        inline constexpr AccessFlag INDIRECT_COMMAND_READ        = Diligent::ACCESS_FLAG_INDIRECT_COMMAND_READ;
        inline constexpr AccessFlag INDEX_READ                   = Diligent::ACCESS_FLAG_INDEX_READ;
        inline constexpr AccessFlag VERTEX_READ                  = Diligent::ACCESS_FLAG_VERTEX_READ;
        inline constexpr AccessFlag UNIFORM_READ                 = Diligent::ACCESS_FLAG_UNIFORM_READ;
        inline constexpr AccessFlag INPUT_ATTACHMENT_READ        = Diligent::ACCESS_FLAG_INPUT_ATTACHMENT_READ;
        inline constexpr AccessFlag SHADER_READ                  = Diligent::ACCESS_FLAG_SHADER_READ;
        inline constexpr AccessFlag SHADER_WRITE                 = Diligent::ACCESS_FLAG_SHADER_WRITE;
        inline constexpr AccessFlag RENDER_TARGET_READ           = Diligent::ACCESS_FLAG_RENDER_TARGET_READ;
        inline constexpr AccessFlag RENDER_TARGET_WRITE          = Diligent::ACCESS_FLAG_RENDER_TARGET_WRITE;
        inline constexpr AccessFlag DEPTH_STENCIL_READ           = Diligent::ACCESS_FLAG_DEPTH_STENCIL_READ;
        inline constexpr AccessFlag DEPTH_STENCIL_WRITE          = Diligent::ACCESS_FLAG_DEPTH_STENCIL_WRITE;
        inline constexpr AccessFlag COPY_SRC                     = Diligent::ACCESS_FLAG_COPY_SRC;
        inline constexpr AccessFlag COPY_DST                     = Diligent::ACCESS_FLAG_COPY_DST;
        inline constexpr AccessFlag HOST_READ                    = Diligent::ACCESS_FLAG_HOST_READ;
        inline constexpr AccessFlag HOST_WRITE                   = Diligent::ACCESS_FLAG_HOST_WRITE;
        inline constexpr AccessFlag MEMORY_READ                  = Diligent::ACCESS_FLAG_MEMORY_READ;
        inline constexpr AccessFlag MEMORY_WRITE                 = Diligent::ACCESS_FLAG_MEMORY_WRITE;
        inline constexpr AccessFlag CONDITIONAL_RENDERING_READ   = Diligent::ACCESS_FLAG_CONDITIONAL_RENDERING_READ;
        inline constexpr AccessFlag SHADING_RATE_TEXTURE_READ    = Diligent::ACCESS_FLAG_SHADING_RATE_TEXTURE_READ;
        inline constexpr AccessFlag ACCELERATION_STRUCTURE_READ  = Diligent::ACCESS_FLAG_ACCELERATION_STRUCTURE_READ;
        inline constexpr AccessFlag ACCELERATION_STRUCTURE_WRITE = Diligent::ACCESS_FLAG_ACCELERATION_STRUCTURE_WRITE;
        inline constexpr AccessFlag FRAGMENT_DENSITY_MAP_READ    = Diligent::ACCESS_FLAG_FRAGMENT_DENSITY_MAP_READ;
        inline constexpr AccessFlag DEFAULT                      = Diligent::ACCESS_FLAG_DEFAULT;
    }

    using ResourceState = Diligent::RESOURCE_STATE;
    namespace ResourceStates {
        inline constexpr ResourceState UNKNOWN           = Diligent::RESOURCE_STATE_UNKNOWN;
        inline constexpr ResourceState UNDEFINED         = Diligent::RESOURCE_STATE_UNDEFINED;
        inline constexpr ResourceState VERTEX_BUFFER     = Diligent::RESOURCE_STATE_VERTEX_BUFFER;
        inline constexpr ResourceState CONSTANT_BUFFER   = Diligent::RESOURCE_STATE_CONSTANT_BUFFER;
        inline constexpr ResourceState INDEX_BUFFER      = Diligent::RESOURCE_STATE_INDEX_BUFFER;
        inline constexpr ResourceState RENDER_TARGET     = Diligent::RESOURCE_STATE_RENDER_TARGET;
        inline constexpr ResourceState UNORDERED_ACCESS  = Diligent::RESOURCE_STATE_UNORDERED_ACCESS;
        inline constexpr ResourceState DEPTH_WRITE       = Diligent::RESOURCE_STATE_DEPTH_WRITE;
        inline constexpr ResourceState DEPTH_READ        = Diligent::RESOURCE_STATE_DEPTH_READ;
        inline constexpr ResourceState SHADER_RESOURCE   = Diligent::RESOURCE_STATE_SHADER_RESOURCE;
        inline constexpr ResourceState STREAM_OUT        = Diligent::RESOURCE_STATE_STREAM_OUT;
        inline constexpr ResourceState INDIRECT_ARGUMENT = Diligent::RESOURCE_STATE_INDIRECT_ARGUMENT;
        inline constexpr ResourceState COPY_DEST         = Diligent::RESOURCE_STATE_COPY_DEST;
        inline constexpr ResourceState COPY_SOURCE       = Diligent::RESOURCE_STATE_COPY_SOURCE;
        inline constexpr ResourceState RESOLVE_DEST      = Diligent::RESOURCE_STATE_RESOLVE_DEST;
        inline constexpr ResourceState RESOLVE_SOURCE    = Diligent::RESOURCE_STATE_RESOLVE_SOURCE;
        inline constexpr ResourceState INPUT_ATTACHMENT  = Diligent::RESOURCE_STATE_INPUT_ATTACHMENT;
        inline constexpr ResourceState PRESENT           = Diligent::RESOURCE_STATE_PRESENT;
        inline constexpr ResourceState BUILD_AS_READ     = Diligent::RESOURCE_STATE_BUILD_AS_READ;
        inline constexpr ResourceState BUILD_AS_WRITE    = Diligent::RESOURCE_STATE_BUILD_AS_WRITE;
        inline constexpr ResourceState RAY_TRACING       = Diligent::RESOURCE_STATE_RAY_TRACING;
        inline constexpr ResourceState COMMON            = Diligent::RESOURCE_STATE_COMMON;
        inline constexpr ResourceState SHADING_RATE      = Diligent::RESOURCE_STATE_SHADING_RATE;
        inline constexpr ResourceState MAX_BIT           = Diligent::RESOURCE_STATE_MAX_BIT;
        inline constexpr ResourceState GENERIC_READ      = Diligent::RESOURCE_STATE_GENERIC_READ;
    }

    using StateTransitionType = Diligent::STATE_TRANSITION_TYPE;
    namespace StateTransitionTypes {
        inline constexpr StateTransitionType IMMEDIATE = Diligent::STATE_TRANSITION_TYPE_IMMEDIATE;
        inline constexpr StateTransitionType BEGIN     = Diligent::STATE_TRANSITION_TYPE_BEGIN;
        inline constexpr StateTransitionType END       = Diligent::STATE_TRANSITION_TYPE_END;
    }

    // InputLayout.h
    using InputElementFrequency = Diligent::INPUT_ELEMENT_FREQUENCY;
    namespace InputElementFrequencys {
        inline constexpr InputElementFrequency UNDEFINED       = Diligent::INPUT_ELEMENT_FREQUENCY_UNDEFINED;
        inline constexpr InputElementFrequency PER_VERTEX      = Diligent::INPUT_ELEMENT_FREQUENCY_PER_VERTEX;
        inline constexpr InputElementFrequency PER_INSTANCE    = Diligent::INPUT_ELEMENT_FREQUENCY_PER_INSTANCE;
        inline constexpr InputElementFrequency NUM_FREQUENCIES = Diligent::INPUT_ELEMENT_FREQUENCY_NUM_FREQUENCIES;
    }

    // PipelineResourceSignature.h
    using PipelineResourceFlag = Diligent::PIPELINE_RESOURCE_FLAGS;
    namespace PipelineResourceFlags {
        inline constexpr PipelineResourceFlag NONE                     = Diligent::PIPELINE_RESOURCE_FLAG_NONE;
        inline constexpr PipelineResourceFlag NO_DYNAMIC_BUFFERS       = Diligent::PIPELINE_RESOURCE_FLAG_NO_DYNAMIC_BUFFERS;
        inline constexpr PipelineResourceFlag INLINE_CONSTANTS         = Diligent::PIPELINE_RESOURCE_FLAG_INLINE_CONSTANTS;
        inline constexpr PipelineResourceFlag COMBINED_SAMPLER         = Diligent::PIPELINE_RESOURCE_FLAG_COMBINED_SAMPLER;
        inline constexpr PipelineResourceFlag FORMATTED_BUFFER         = Diligent::PIPELINE_RESOURCE_FLAG_FORMATTED_BUFFER;
        inline constexpr PipelineResourceFlag RUNTIME_ARRAY            = Diligent::PIPELINE_RESOURCE_FLAG_RUNTIME_ARRAY;
        inline constexpr PipelineResourceFlag GENERAL_INPUT_ATTACHMENT = Diligent::PIPELINE_RESOURCE_FLAG_GENERAL_INPUT_ATTACHMENT;
        inline constexpr PipelineResourceFlag LAST                     = Diligent::PIPELINE_RESOURCE_FLAG_LAST;
    }

    using WebGpuBindingType = Diligent::WEB_GPU_BINDING_TYPE;
    namespace WebGpuBindingTypes {
        inline constexpr WebGpuBindingType DEFAULT                       = Diligent::WEB_GPU_BINDING_TYPE_DEFAULT;
        inline constexpr WebGpuBindingType FILTERING_SAMPLER             = Diligent::WEB_GPU_BINDING_TYPE_FILTERING_SAMPLER;
        inline constexpr WebGpuBindingType NON_FILTERING_SAMPLER         = Diligent::WEB_GPU_BINDING_TYPE_NON_FILTERING_SAMPLER;
        inline constexpr WebGpuBindingType COMPARISON_SAMPLER            = Diligent::WEB_GPU_BINDING_TYPE_COMPARISON_SAMPLER;
        inline constexpr WebGpuBindingType FLOAT_TEXTURE                 = Diligent::WEB_GPU_BINDING_TYPE_FLOAT_TEXTURE;
        inline constexpr WebGpuBindingType UNFILTERABLE_FLOAT_TEXTURE    = Diligent::WEB_GPU_BINDING_TYPE_UNFILTERABLE_FLOAT_TEXTURE;
        inline constexpr WebGpuBindingType DEPTH_TEXTURE                 = Diligent::WEB_GPU_BINDING_TYPE_DEPTH_TEXTURE;
        inline constexpr WebGpuBindingType SINT_TEXTURE                  = Diligent::WEB_GPU_BINDING_TYPE_SINT_TEXTURE;
        inline constexpr WebGpuBindingType UINT_TEXTURE                  = Diligent::WEB_GPU_BINDING_TYPE_UINT_TEXTURE;
        inline constexpr WebGpuBindingType FLOAT_TEXTURE_MS              = Diligent::WEB_GPU_BINDING_TYPE_FLOAT_TEXTURE_MS;
        inline constexpr WebGpuBindingType UNFILTERABLE_FLOAT_TEXTURE_MS = Diligent::WEB_GPU_BINDING_TYPE_UNFILTERABLE_FLOAT_TEXTURE_MS;
        inline constexpr WebGpuBindingType DEPTH_TEXTURE_MS              = Diligent::WEB_GPU_BINDING_TYPE_DEPTH_TEXTURE_MS;
        inline constexpr WebGpuBindingType SINT_TEXTURE_MS               = Diligent::WEB_GPU_BINDING_TYPE_SINT_TEXTURE_MS;
        inline constexpr WebGpuBindingType UINT_TEXTURE_MS               = Diligent::WEB_GPU_BINDING_TYPE_UINT_TEXTURE_MS;
        inline constexpr WebGpuBindingType WRITE_ONLY_TEXTURE_UAV        = Diligent::WEB_GPU_BINDING_TYPE_WRITE_ONLY_TEXTURE_UAV;
        inline constexpr WebGpuBindingType READ_ONLY_TEXTURE_UAV         = Diligent::WEB_GPU_BINDING_TYPE_READ_ONLY_TEXTURE_UAV;
        inline constexpr WebGpuBindingType READ_WRITE_TEXTURE_UAV        = Diligent::WEB_GPU_BINDING_TYPE_READ_WRITE_TEXTURE_UAV;
        inline constexpr WebGpuBindingType COUNT                         = Diligent::WEB_GPU_BINDING_TYPE_COUNT;
    }

    // PipelineStateCache.h
    using PsoCacheMode = Diligent::PSO_CACHE_MODE;
    namespace PsoCacheModes {
        inline constexpr PsoCacheMode LOAD       = Diligent::PSO_CACHE_MODE_LOAD;
        inline constexpr PsoCacheMode STORE      = Diligent::PSO_CACHE_MODE_STORE;
        inline constexpr PsoCacheMode LOAD_STORE = Diligent::PSO_CACHE_MODE_LOAD_STORE;
    }

    using PsoCacheFlag = Diligent::PSO_CACHE_FLAGS;
    namespace PsoCacheFlags {
        inline constexpr PsoCacheFlag NONE    = Diligent::PSO_CACHE_FLAG_NONE;
        inline constexpr PsoCacheFlag VERBOSE = Diligent::PSO_CACHE_FLAG_VERBOSE;
    }

    // PipelineState.h
    using ShaderVariableFlag = Diligent::SHADER_VARIABLE_FLAGS;
    namespace ShaderVariableFlags {
        inline constexpr ShaderVariableFlag NONE                        = Diligent::SHADER_VARIABLE_FLAG_NONE;
        inline constexpr ShaderVariableFlag NO_DYNAMIC_BUFFERS          = Diligent::SHADER_VARIABLE_FLAG_NO_DYNAMIC_BUFFERS;
        inline constexpr ShaderVariableFlag INLINE_CONSTANTS            = Diligent::SHADER_VARIABLE_FLAG_INLINE_CONSTANTS;
        inline constexpr ShaderVariableFlag GENERAL_INPUT_ATTACHMENT_VK = Diligent::SHADER_VARIABLE_FLAG_GENERAL_INPUT_ATTACHMENT_VK;
        inline constexpr ShaderVariableFlag UNFILTERABLE_FLOAT_TEXTURE_WEBGPU = Diligent::SHADER_VARIABLE_FLAG_UNFILTERABLE_FLOAT_TEXTURE_WEBGPU;
        inline constexpr ShaderVariableFlag NON_FILTERING_SAMPLER_WEBGPU = Diligent::SHADER_VARIABLE_FLAG_NON_FILTERING_SAMPLER_WEBGPU;
        inline constexpr ShaderVariableFlag LAST                         = Diligent::SHADER_VARIABLE_FLAG_LAST;
    }

    using PipelineShadingRateFlag = Diligent::PIPELINE_SHADING_RATE_FLAGS;
    namespace PipelineShadingRateFlags {
        inline constexpr PipelineShadingRateFlag NONE          = Diligent::PIPELINE_SHADING_RATE_FLAG_NONE;
        inline constexpr PipelineShadingRateFlag PER_PRIMITIVE = Diligent::PIPELINE_SHADING_RATE_FLAG_PER_PRIMITIVE;
        inline constexpr PipelineShadingRateFlag TEXTURE_BASED = Diligent::PIPELINE_SHADING_RATE_FLAG_TEXTURE_BASED;
        inline constexpr PipelineShadingRateFlag LAST          = Diligent::PIPELINE_SHADING_RATE_FLAG_LAST;
    }

    using PipelineType = Diligent::PIPELINE_TYPE;
    namespace PipelineTypes {
        inline constexpr PipelineType GRAPHICS    = Diligent::PIPELINE_TYPE_GRAPHICS;
        inline constexpr PipelineType COMPUTE     = Diligent::PIPELINE_TYPE_COMPUTE;
        inline constexpr PipelineType MESH        = Diligent::PIPELINE_TYPE_MESH;
        inline constexpr PipelineType RAY_TRACING = Diligent::PIPELINE_TYPE_RAY_TRACING;
        inline constexpr PipelineType TILE        = Diligent::PIPELINE_TYPE_TILE;
        inline constexpr PipelineType LAST        = Diligent::PIPELINE_TYPE_LAST;
        inline constexpr PipelineType COUNT       = Diligent::PIPELINE_TYPE_COUNT;
        inline constexpr PipelineType INVALID     = Diligent::PIPELINE_TYPE_INVALID;
    }

    using PsoCreateFlag = Diligent::PSO_CREATE_FLAGS;
    namespace PsoCreateFlags {
        inline constexpr PsoCreateFlag NONE                              = Diligent::PSO_CREATE_FLAG_NONE;
        inline constexpr PsoCreateFlag IGNORE_MISSING_VARIABLES          = Diligent::PSO_CREATE_FLAG_IGNORE_MISSING_VARIABLES;
        inline constexpr PsoCreateFlag IGNORE_MISSING_IMMUTABLE_SAMPLERS = Diligent::PSO_CREATE_FLAG_IGNORE_MISSING_IMMUTABLE_SAMPLERS;
        inline constexpr PsoCreateFlag DONT_REMAP_SHADER_RESOURCES       = Diligent::PSO_CREATE_FLAG_DONT_REMAP_SHADER_RESOURCES;
        inline constexpr PsoCreateFlag ASYNCHRONOUS                      = Diligent::PSO_CREATE_FLAG_ASYNCHRONOUS;
        inline constexpr PsoCreateFlag LAST                              = Diligent::PSO_CREATE_FLAG_LAST;
    }

    using PipelineStateStatu = Diligent::PIPELINE_STATE_STATUS;
    namespace PipelineStateStatus {
        inline constexpr PipelineStateStatu UNINITIALIZED = Diligent::PIPELINE_STATE_STATUS_UNINITIALIZED;
        inline constexpr PipelineStateStatu COMPILING     = Diligent::PIPELINE_STATE_STATUS_COMPILING;
        inline constexpr PipelineStateStatu READY         = Diligent::PIPELINE_STATE_STATUS_READY;
        inline constexpr PipelineStateStatu FAILED        = Diligent::PIPELINE_STATE_STATUS_FAILED;
    }

    // RasterizerState.h
    using FillMode = Diligent::FILL_MODE;
    namespace FillModes {
        inline constexpr FillMode UNDEFINED = Diligent::FILL_MODE_UNDEFINED;
        inline constexpr FillMode WIREFRAME = Diligent::FILL_MODE_WIREFRAME;
        inline constexpr FillMode SOLID     = Diligent::FILL_MODE_SOLID;
        inline constexpr FillMode NUM_MODES = Diligent::FILL_MODE_NUM_MODES;
    }

    using CullMode = Diligent::CULL_MODE;
    namespace CullModes {
        inline constexpr CullMode UNDEFINED = Diligent::CULL_MODE_UNDEFINED;
        inline constexpr CullMode NONE      = Diligent::CULL_MODE_NONE;
        inline constexpr CullMode FRONT     = Diligent::CULL_MODE_FRONT;
        inline constexpr CullMode BACK      = Diligent::CULL_MODE_BACK;
        inline constexpr CullMode NUM_MODES = Diligent::CULL_MODE_NUM_MODES;
    }

    // RenderPass.h
    using AttachmentLoadOp = Diligent::ATTACHMENT_LOAD_OP;
    namespace AttachmentLoadOps {
        inline constexpr AttachmentLoadOp LOAD    = Diligent::ATTACHMENT_LOAD_OP_LOAD;
        inline constexpr AttachmentLoadOp CLEAR   = Diligent::ATTACHMENT_LOAD_OP_CLEAR;
        inline constexpr AttachmentLoadOp DISCARD = Diligent::ATTACHMENT_LOAD_OP_DISCARD;
        inline constexpr AttachmentLoadOp COUNT   = Diligent::ATTACHMENT_LOAD_OP_COUNT;
    }

    using AttachmentStoreOp = Diligent::ATTACHMENT_STORE_OP;
    namespace AttachmentStoreOps {
        inline constexpr AttachmentStoreOp STORE   = Diligent::ATTACHMENT_STORE_OP_STORE;
        inline constexpr AttachmentStoreOp DISCARD = Diligent::ATTACHMENT_STORE_OP_DISCARD;
        inline constexpr AttachmentStoreOp COUNT   = Diligent::ATTACHMENT_STORE_OP_COUNT;
    }

    // Sampler.h
    using SamplerFlag = Diligent::SAMPLER_FLAGS;
    namespace SamplerFlags {
        inline constexpr SamplerFlag NONE                             = Diligent::SAMPLER_FLAG_NONE;
        inline constexpr SamplerFlag SUBSAMPLED                       = Diligent::SAMPLER_FLAG_SUBSAMPLED;
        inline constexpr SamplerFlag SUBSAMPLED_COARSE_RECONSTRUCTION = Diligent::SAMPLER_FLAG_SUBSAMPLED_COARSE_RECONSTRUCTION;
        inline constexpr SamplerFlag LAST                             = Diligent::SAMPLER_FLAG_LAST;
    }

    // ShaderBindingTable.h
    using VerifySbtFlag = Diligent::VERIFY_SBT_FLAGS;
    namespace VerifySbtFlags {
        inline constexpr VerifySbtFlag SHADER_ONLY   = Diligent::VERIFY_SBT_FLAG_SHADER_ONLY;
        inline constexpr VerifySbtFlag SHADER_RECORD = Diligent::VERIFY_SBT_FLAG_SHADER_RECORD;
        inline constexpr VerifySbtFlag TLAS          = Diligent::VERIFY_SBT_FLAG_TLAS;
        inline constexpr VerifySbtFlag ALL           = Diligent::VERIFY_SBT_FLAG_ALL;
    }

    // Shader.h
    using ShaderSourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE;
    namespace ShaderSourceLanguages {
        inline constexpr ShaderSourceLanguage DEFAULT       = Diligent::SHADER_SOURCE_LANGUAGE_DEFAULT;
        inline constexpr ShaderSourceLanguage HLSL          = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
        inline constexpr ShaderSourceLanguage GLSL          = Diligent::SHADER_SOURCE_LANGUAGE_GLSL;
        inline constexpr ShaderSourceLanguage GLSL_VERBATIM = Diligent::SHADER_SOURCE_LANGUAGE_GLSL_VERBATIM;
        inline constexpr ShaderSourceLanguage MSL           = Diligent::SHADER_SOURCE_LANGUAGE_MSL;
        inline constexpr ShaderSourceLanguage MSL_VERBATIM  = Diligent::SHADER_SOURCE_LANGUAGE_MSL_VERBATIM;
        inline constexpr ShaderSourceLanguage MTLB          = Diligent::SHADER_SOURCE_LANGUAGE_MTLB;
        inline constexpr ShaderSourceLanguage WGSL          = Diligent::SHADER_SOURCE_LANGUAGE_WGSL;
        inline constexpr ShaderSourceLanguage BYTECODE      = Diligent::SHADER_SOURCE_LANGUAGE_BYTECODE;
        inline constexpr ShaderSourceLanguage COUNT         = Diligent::SHADER_SOURCE_LANGUAGE_COUNT;
    }

    using ShaderCompiler = Diligent::SHADER_COMPILER;
    namespace ShaderCompilers {
        inline constexpr ShaderCompiler DEFAULT = Diligent::SHADER_COMPILER_DEFAULT;
        inline constexpr ShaderCompiler GLSLANG = Diligent::SHADER_COMPILER_GLSLANG;
        inline constexpr ShaderCompiler DXC     = Diligent::SHADER_COMPILER_DXC;
        inline constexpr ShaderCompiler FXC     = Diligent::SHADER_COMPILER_FXC;
        inline constexpr ShaderCompiler LAST    = Diligent::SHADER_COMPILER_LAST;
        inline constexpr ShaderCompiler COUNT   = Diligent::SHADER_COMPILER_COUNT;
    }

    using CreateShaderSourceInputStreamFlag = Diligent::CREATE_SHADER_SOURCE_INPUT_STREAM_FLAGS;
    namespace CreateShaderSourceInputStreamFlags {
        inline constexpr CreateShaderSourceInputStreamFlag NONE   = Diligent::CREATE_SHADER_SOURCE_INPUT_STREAM_FLAG_NONE;
        inline constexpr CreateShaderSourceInputStreamFlag SILENT = Diligent::CREATE_SHADER_SOURCE_INPUT_STREAM_FLAG_SILENT;
    }

    using ShaderStatu = Diligent::SHADER_STATUS;
    namespace ShaderStatus {
        inline constexpr ShaderStatu UNINITIALIZED = Diligent::SHADER_STATUS_UNINITIALIZED;
        inline constexpr ShaderStatu COMPILING     = Diligent::SHADER_STATUS_COMPILING;
        inline constexpr ShaderStatu READY         = Diligent::SHADER_STATUS_READY;
        inline constexpr ShaderStatu FAILED        = Diligent::SHADER_STATUS_FAILED;
    }

    using ShaderCompileFlag = Diligent::SHADER_COMPILE_FLAGS;
    namespace ShaderCompileFlags {
        inline constexpr ShaderCompileFlag NONE                    = Diligent::SHADER_COMPILE_FLAG_NONE;
        inline constexpr ShaderCompileFlag ENABLE_UNBOUNDED_ARRAYS = Diligent::SHADER_COMPILE_FLAG_ENABLE_UNBOUNDED_ARRAYS;
        inline constexpr ShaderCompileFlag SKIP_REFLECTION         = Diligent::SHADER_COMPILE_FLAG_SKIP_REFLECTION;
        inline constexpr ShaderCompileFlag ASYNCHRONOUS            = Diligent::SHADER_COMPILE_FLAG_ASYNCHRONOUS;
        inline constexpr ShaderCompileFlag PACK_MATRIX_ROW_MAJOR   = Diligent::SHADER_COMPILE_FLAG_PACK_MATRIX_ROW_MAJOR;
        inline constexpr ShaderCompileFlag HLSL_TO_SPIRV_VIA_GLSL  = Diligent::SHADER_COMPILE_FLAG_HLSL_TO_SPIRV_VIA_GLSL;
        inline constexpr ShaderCompileFlag LAST                    = Diligent::SHADER_COMPILE_FLAG_LAST;
    }

    using ShaderResourceType = Diligent::SHADER_RESOURCE_TYPE;
    namespace ShaderResourceTypes {
        inline constexpr ShaderResourceType UNKNOWN          = Diligent::SHADER_RESOURCE_TYPE_UNKNOWN;
        inline constexpr ShaderResourceType CONSTANT_BUFFER  = Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER;
        inline constexpr ShaderResourceType TEXTURE_SRV      = Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV;
        inline constexpr ShaderResourceType BUFFER_SRV       = Diligent::SHADER_RESOURCE_TYPE_BUFFER_SRV;
        inline constexpr ShaderResourceType TEXTURE_UAV      = Diligent::SHADER_RESOURCE_TYPE_TEXTURE_UAV;
        inline constexpr ShaderResourceType BUFFER_UAV       = Diligent::SHADER_RESOURCE_TYPE_BUFFER_UAV;
        inline constexpr ShaderResourceType SAMPLER          = Diligent::SHADER_RESOURCE_TYPE_SAMPLER;
        inline constexpr ShaderResourceType INPUT_ATTACHMENT = Diligent::SHADER_RESOURCE_TYPE_INPUT_ATTACHMENT;
        inline constexpr ShaderResourceType ACCEL_STRUCT     = Diligent::SHADER_RESOURCE_TYPE_ACCEL_STRUCT;
        inline constexpr ShaderResourceType LAST             = Diligent::SHADER_RESOURCE_TYPE_LAST;
    }

    using ShaderCodeBasicType = Diligent::SHADER_CODE_BASIC_TYPE;
    namespace ShaderCodeBasicTypes {
        inline constexpr ShaderCodeBasicType UNKNOWN    = Diligent::SHADER_CODE_BASIC_TYPE_UNKNOWN;
        inline constexpr ShaderCodeBasicType VOID       = Diligent::SHADER_CODE_BASIC_TYPE_VOID;
        inline constexpr ShaderCodeBasicType BOOL       = Diligent::SHADER_CODE_BASIC_TYPE_BOOL;
        inline constexpr ShaderCodeBasicType INT        = Diligent::SHADER_CODE_BASIC_TYPE_INT;
        inline constexpr ShaderCodeBasicType INT8       = Diligent::SHADER_CODE_BASIC_TYPE_INT8;
        inline constexpr ShaderCodeBasicType INT16      = Diligent::SHADER_CODE_BASIC_TYPE_INT16;
        inline constexpr ShaderCodeBasicType INT64      = Diligent::SHADER_CODE_BASIC_TYPE_INT64;
        inline constexpr ShaderCodeBasicType UINT       = Diligent::SHADER_CODE_BASIC_TYPE_UINT;
        inline constexpr ShaderCodeBasicType UINT8      = Diligent::SHADER_CODE_BASIC_TYPE_UINT8;
        inline constexpr ShaderCodeBasicType UINT16     = Diligent::SHADER_CODE_BASIC_TYPE_UINT16;
        inline constexpr ShaderCodeBasicType UINT64     = Diligent::SHADER_CODE_BASIC_TYPE_UINT64;
        inline constexpr ShaderCodeBasicType FLOAT      = Diligent::SHADER_CODE_BASIC_TYPE_FLOAT;
        inline constexpr ShaderCodeBasicType FLOAT16    = Diligent::SHADER_CODE_BASIC_TYPE_FLOAT16;
        inline constexpr ShaderCodeBasicType DOUBLE     = Diligent::SHADER_CODE_BASIC_TYPE_DOUBLE;
        inline constexpr ShaderCodeBasicType MIN8FLOAT  = Diligent::SHADER_CODE_BASIC_TYPE_MIN8FLOAT;
        inline constexpr ShaderCodeBasicType MIN10FLOAT = Diligent::SHADER_CODE_BASIC_TYPE_MIN10FLOAT;
        inline constexpr ShaderCodeBasicType MIN16FLOAT = Diligent::SHADER_CODE_BASIC_TYPE_MIN16FLOAT;
        inline constexpr ShaderCodeBasicType MIN12INT   = Diligent::SHADER_CODE_BASIC_TYPE_MIN12INT;
        inline constexpr ShaderCodeBasicType MIN16INT   = Diligent::SHADER_CODE_BASIC_TYPE_MIN16INT;
        inline constexpr ShaderCodeBasicType MIN16UINT  = Diligent::SHADER_CODE_BASIC_TYPE_MIN16UINT;
        inline constexpr ShaderCodeBasicType STRING     = Diligent::SHADER_CODE_BASIC_TYPE_STRING;
        inline constexpr ShaderCodeBasicType COUNT      = Diligent::SHADER_CODE_BASIC_TYPE_COUNT;
    }

    using ShaderCodeVariableClas = Diligent::SHADER_CODE_VARIABLE_CLASS;
    namespace ShaderCodeVariableClass {
        inline constexpr ShaderCodeVariableClas UNKNOWN        = Diligent::SHADER_CODE_VARIABLE_CLASS_UNKNOWN;
        inline constexpr ShaderCodeVariableClas SCALAR         = Diligent::SHADER_CODE_VARIABLE_CLASS_SCALAR;
        inline constexpr ShaderCodeVariableClas VECTOR         = Diligent::SHADER_CODE_VARIABLE_CLASS_VECTOR;
        inline constexpr ShaderCodeVariableClas MATRIX_ROWS    = Diligent::SHADER_CODE_VARIABLE_CLASS_MATRIX_ROWS;
        inline constexpr ShaderCodeVariableClas MATRIX_COLUMNS = Diligent::SHADER_CODE_VARIABLE_CLASS_MATRIX_COLUMNS;
        inline constexpr ShaderCodeVariableClas STRUCT         = Diligent::SHADER_CODE_VARIABLE_CLASS_STRUCT;
        inline constexpr ShaderCodeVariableClas COUNT          = Diligent::SHADER_CODE_VARIABLE_CLASS_COUNT;
    }

    // ShaderResourceVariable.h
    using ShaderResourceVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE;
    namespace ShaderResourceVariableTypes {
        inline constexpr ShaderResourceVariableType STATIC    = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        inline constexpr ShaderResourceVariableType MUTABLE   = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;
        inline constexpr ShaderResourceVariableType DYNAMIC   = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;
        inline constexpr ShaderResourceVariableType NUM_TYPES = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES;
    }

    using ShaderResourceVariableTypeFlag = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAGS;
    namespace ShaderResourceVariableTypeFlags {
        inline constexpr ShaderResourceVariableTypeFlag NONE    = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAG_NONE;
        inline constexpr ShaderResourceVariableTypeFlag STATIC  = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAG_STATIC;
        inline constexpr ShaderResourceVariableTypeFlag MUTABLE = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAG_MUTABLE;
        inline constexpr ShaderResourceVariableTypeFlag DYNAMIC = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAG_DYNAMIC;
        inline constexpr ShaderResourceVariableTypeFlag MUT_DYN = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAG_MUT_DYN;
        inline constexpr ShaderResourceVariableTypeFlag ALL     = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_FLAG_ALL;
    }

    using BindShaderResourcesFlag = Diligent::BIND_SHADER_RESOURCES_FLAGS;
    namespace BindShaderResourcesFlags {
        inline constexpr BindShaderResourcesFlag UPDATE_STATIC       = Diligent::BIND_SHADER_RESOURCES_UPDATE_STATIC;
        inline constexpr BindShaderResourcesFlag UPDATE_MUTABLE      = Diligent::BIND_SHADER_RESOURCES_UPDATE_MUTABLE;
        inline constexpr BindShaderResourcesFlag UPDATE_DYNAMIC      = Diligent::BIND_SHADER_RESOURCES_UPDATE_DYNAMIC;
        inline constexpr BindShaderResourcesFlag UPDATE_ALL          = Diligent::BIND_SHADER_RESOURCES_UPDATE_ALL;
        inline constexpr BindShaderResourcesFlag KEEP_EXISTING       = Diligent::BIND_SHADER_RESOURCES_KEEP_EXISTING;
        inline constexpr BindShaderResourcesFlag VERIFY_ALL_RESOLVED = Diligent::BIND_SHADER_RESOURCES_VERIFY_ALL_RESOLVED;
        inline constexpr BindShaderResourcesFlag ALLOW_OVERWRITE     = Diligent::BIND_SHADER_RESOURCES_ALLOW_OVERWRITE;
    }

    using SetShaderResourceFlag = Diligent::SET_SHADER_RESOURCE_FLAGS;
    namespace SetShaderResourceFlags {
        inline constexpr SetShaderResourceFlag NONE            = Diligent::SET_SHADER_RESOURCE_FLAG_NONE;
        inline constexpr SetShaderResourceFlag ALLOW_OVERWRITE = Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE;
    }

    // Texture.h
    using MiscTextureFlag = Diligent::MISC_TEXTURE_FLAGS;
    namespace MiscTextureFlags {
        inline constexpr MiscTextureFlag NONE            = Diligent::MISC_TEXTURE_FLAG_NONE;
        inline constexpr MiscTextureFlag GENERATE_MIPS   = Diligent::MISC_TEXTURE_FLAG_GENERATE_MIPS;
        inline constexpr MiscTextureFlag MEMORYLESS      = Diligent::MISC_TEXTURE_FLAG_MEMORYLESS;
        inline constexpr MiscTextureFlag SPARSE_ALIASING = Diligent::MISC_TEXTURE_FLAG_SPARSE_ALIASING;
        inline constexpr MiscTextureFlag SUBSAMPLED      = Diligent::MISC_TEXTURE_FLAG_SUBSAMPLED;
    }

    // TextureView.h
    using UavAccessFlag = Diligent::UAV_ACCESS_FLAG;
    namespace UavAccessFlags {
        inline constexpr UavAccessFlag UNSPECIFIED = Diligent::UAV_ACCESS_UNSPECIFIED;
        inline constexpr UavAccessFlag READ        = Diligent::UAV_ACCESS_FLAG_READ;
        inline constexpr UavAccessFlag WRITE       = Diligent::UAV_ACCESS_FLAG_WRITE;
        inline constexpr UavAccessFlag READ_WRITE  = Diligent::UAV_ACCESS_FLAG_READ_WRITE;
        inline constexpr UavAccessFlag LAST        = Diligent::UAV_ACCESS_FLAG_LAST;
    }

    using TextureViewFlag = Diligent::TEXTURE_VIEW_FLAGS;
    namespace TextureViewFlags {
        inline constexpr TextureViewFlag NONE                     = Diligent::TEXTURE_VIEW_FLAG_NONE;
        inline constexpr TextureViewFlag ALLOW_MIP_MAP_GENERATION = Diligent::TEXTURE_VIEW_FLAG_ALLOW_MIP_MAP_GENERATION;
        inline constexpr TextureViewFlag LAST                     = Diligent::TEXTURE_VIEW_FLAG_LAST;
    }

    using TextureComponentSwizzle = Diligent::TEXTURE_COMPONENT_SWIZZLE;
    namespace TextureComponentSwizzles {
        inline constexpr TextureComponentSwizzle IDENTITY = Diligent::TEXTURE_COMPONENT_SWIZZLE_IDENTITY;
        inline constexpr TextureComponentSwizzle ZERO     = Diligent::TEXTURE_COMPONENT_SWIZZLE_ZERO;
        inline constexpr TextureComponentSwizzle ONE      = Diligent::TEXTURE_COMPONENT_SWIZZLE_ONE;
        inline constexpr TextureComponentSwizzle R        = Diligent::TEXTURE_COMPONENT_SWIZZLE_R;
        inline constexpr TextureComponentSwizzle G        = Diligent::TEXTURE_COMPONENT_SWIZZLE_G;
        inline constexpr TextureComponentSwizzle B        = Diligent::TEXTURE_COMPONENT_SWIZZLE_B;
        inline constexpr TextureComponentSwizzle A        = Diligent::TEXTURE_COMPONENT_SWIZZLE_A;
        inline constexpr TextureComponentSwizzle COUNT    = Diligent::TEXTURE_COMPONENT_SWIZZLE_COUNT;
    }

    // TopLevelAS.h
    using HitGroupBindingMode = Diligent::HIT_GROUP_BINDING_MODE;
    namespace HitGroupBindingModes {
        inline constexpr HitGroupBindingMode PER_GEOMETRY = Diligent::HIT_GROUP_BINDING_MODE_PER_GEOMETRY;
        inline constexpr HitGroupBindingMode PER_INSTANCE = Diligent::HIT_GROUP_BINDING_MODE_PER_INSTANCE;
        inline constexpr HitGroupBindingMode PER_TLAS     = Diligent::HIT_GROUP_BINDING_MODE_PER_TLAS;
        inline constexpr HitGroupBindingMode USER_DEFINED = Diligent::HIT_GROUP_BINDING_MODE_USER_DEFINED;
        inline constexpr HitGroupBindingMode LAST         = Diligent::HIT_GROUP_BINDING_MODE_LAST;
    }
}