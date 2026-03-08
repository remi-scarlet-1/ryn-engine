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
    ENTT_ID_TYPE=uint32_t
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
    source/Prelude.h

    PUBLIC FILE_SET CXX_MODULES BASE_DIRS source FILES
    source/container/__BUNDLE__.ixx
    source/container/Box.ixx
    source/container/Identity.ixx
    source/container/Span.ixx

    source/core/__BUNDLE__.ixx
    source/core/Application.ixx
    source/core/Memory.ixx
    source/core/Service.ixx
    source/core/ServiceEvent.ixx
    source/core/Time.ixx

    source/game/__BUNDLE__.ixx
    source/game/Hierarchy.ixx

    source/primitive/__BUNDLE__.ixx
    source/primitive/Graphics.ixx
    source/primitive/Keycode.ixx
    source/primitive/Math.ixx

    source/reflect/__BUNDLE__.ixx
    source/reflect/TypeCommon.ixx
    source/reflect/TypeReflection.ixx

    source/service/__BUNDLE__.ixx
    source/service/Graphics.ixx
    source/service/Input.ixx
    source/service/Window.ixx

    source/service/world/__BUNDLE__.ixx
    source/service/world/World.ixx

    source/Logger.ixx
    source/Ryntime.ixx

    PRIVATE
    source/core/Application.cpp
    source/core/Service.cpp
    source/core/ServiceEvent.cpp

    source/reflect/TypeReflection.cpp

    source/service/Graphics.cpp
    source/service/Input.cpp
    source/service/Window.cpp

    source/service/world/World.cpp

    source/Logger.cpp
)