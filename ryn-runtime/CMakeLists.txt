project(RynRuntime)

add_library(RynRuntime SHARED)
ryn_compile_options(RynRuntime)

set_target_properties(RynRuntime PROPERTIES
    # Hides shared lib symbols on GCC/Clang by default
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_compile_definitions(RynRuntime
    PUBLIC
    # Configs
    $<$<CONFIG:Debug>:RYN_CONFIG_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:RYN_CONFIG_RELEASE>
    $<$<CONFIG:Release>:RYN_CONFIG_DIST>
    # Platforms
    $<$<PLATFORM_ID:Windows>:RYN_PLATFORM_WIN>
    $<$<PLATFORM_ID:Linux>:RYN_PLATFORM_LINUX>
    # Library Options
    GLM_FORCE_PURE
    GLM_FORCE_ALIGNED
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_FORCE_LEFT_HANDED
    ENTT_ID_TYPE=uint64_t
    PRIVATE
    RYN_RUNTIME
)

target_include_directories(RynRuntime
    PUBLIC source
    PRIVATE source
)

target_link_libraries(RynRuntime
    PUBLIC
    Diligent-BuildSettings # public required as this changes compilation options and architecture flags
    Diligent-GraphicsTools # public required as templates like Diligent::MapHelper<T> use debug logic/asserts defined in Diligents headers
    PRIVATE
    spdlog::spdlog
    glm::glm
    SDL3::SDL3
    EnTT::EnTT
    Diligent-GraphicsEngine
    Diligent-GraphicsEngineVk-static
    $<$<PLATFORM_ID:Windows>:Diligent-GraphicsEngineD3D12-static>
    $<$<PLATFORM_ID:Windows>:Diligent-GraphicsEngineD3D11-static>
)

target_sources(RynRuntime
    PUBLIC FILE_SET HEADERS BASE_DIRS source FILES
    source/Defines.h
    source/Entrypoint.h
    source/Macros.h
    source/Prelude.h

    PUBLIC FILE_SET CXX_MODULES BASE_DIRS source FILES
    source/core/container/__BUNDLE__.ixx
    source/core/container/Box.ixx
    source/core/container/Span.ixx

    source/core/primitive/__BUNDLE__.ixx
    source/core/primitive/Graphics.ixx
    source/core/primitive/Keycodes.ixx
    source/core/primitive/Math.ixx
    source/core/primitive/Time.ixx

    source/core/reflection/__BUNDLE__.ixx
    source/core/reflection/TypeCommon.ixx
    source/core/reflection/TypeReflection.ixx

    source/core/__BUNDLE__.ixx
    source/core/Application.ixx
    source/core/Event.ixx
    source/core/Service.ixx

    source/platform/__BUNDLE__.ixx
    source/platform/Graphics.ixx
    source/platform/Input.ixx
    source/platform/Window.ixx

    source/world/__BUNDLE__.ixx
    source/world/World.ixx

    source/Log.ixx
    source/Ryntime.ixx

    PRIVATE
    source/core/reflection/TypeReflection.cpp

    source/core/Application.cpp
    source/core/Event.cpp
    source/core/Service.cpp

    source/platform/Graphics.cpp
    source/platform/Input.cpp
    source/platform/Window.cpp

    source/world/World.cpp

    source/Log.cpp
)