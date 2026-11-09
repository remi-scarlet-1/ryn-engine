add_executable(RynRuntime
    source/Main.cpp
    source/Log.cpp
    source/DynamicLibrary.cpp
    source/PluginHost.cpp
)

target_include_directories(RynRuntime
    PUBLIC include
    PRIVATE source
)

target_link_libraries(RynRuntime PRIVATE RynCommon)
target_compile_definitions(RynRuntime PRIVATE RYN_BUILDING_RUNTIME)

# Plugins link against the executable to call RYN_API functions.
# Windows: generates an import library. Linux: exports symbols dynamically (-rdynamic).
set_target_properties(RynRuntime PROPERTIES ENABLE_EXPORTS ON)

if(RYN_PLATFORM_WINDOWS)
    # libstdc++ keeps std::print's Windows console support in a separate library
    target_link_libraries(RynRuntime PRIVATE stdc++exp)
elseif(RYN_PLATFORM_LINUX)
    target_link_libraries(RynRuntime PRIVATE ${CMAKE_DL_LIBS})
endif()
