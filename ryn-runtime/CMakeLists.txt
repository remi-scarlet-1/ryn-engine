add_executable(RynRuntime)

target_sources(RynRuntime
    # Headers consumers may include (mainly macro-only)
    PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
    include/ryn/Defines.h
    include/ryn/Plugin.h

    # Modules consumers may import
    PUBLIC FILE_SET CXX_MODULES BASE_DIRS source FILES
    source/Ryn.ixx
    source/core/_Mod.ixx
    source/core/Types.ixx
    source/core/Log.ixx
    source/core/Plugin.ixx

    # Internal modules, invisible to consumers
    PRIVATE FILE_SET internal_modules TYPE CXX_MODULES BASE_DIRS source FILES
    source/internal/_Mod.ixx
    source/internal/DynamicLibrary.ixx
    source/internal/PluginHost.ixx

    PRIVATE
    source/Main.cpp
    source/core/Log.cpp
    source/internal/DynamicLibrary.cpp
    source/internal/PluginHost.cpp
)

target_link_libraries(RynRuntime PRIVATE RynCommon)
target_compile_definitions(RynRuntime PRIVATE RYN_BUILDING_RUNTIME)

# Enable plugins to link against the executable so they can call RYN_API functions.
set_target_properties(RynRuntime PROPERTIES ENABLE_EXPORTS ON)

if(RYN_PLATFORM_LINUX)
    target_link_libraries(RynRuntime PRIVATE ${CMAKE_DL_LIBS})
endif()
