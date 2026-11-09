# Shared settings for every Ryn target (runtime and plugins)
add_library(RynCommon INTERFACE)

target_compile_options(RynCommon INTERFACE
    -freflection
    -fno-exceptions -fno-rtti
    -Wall -Wextra -Wpedantic -Werror
)

target_compile_definitions(RynCommon INTERFACE
    # Configs
    $<$<CONFIG:Debug>:RYN_CONFIG_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:RYN_CONFIG_RELEASE>
    $<$<CONFIG:Release>:RYN_CONFIG_DIST>
    # Platforms
    $<$<BOOL:${RYN_PLATFORM_WINDOWS}>:RYN_PLATFORM_WINDOWS>
    $<$<BOOL:${RYN_PLATFORM_LINUX}>:RYN_PLATFORM_LINUX>
)

# Declares a plugin (Editor, Game, ...) that the runtime loads at startup.
# Produces "<name>.dll" on Windows and "lib<name>.so" on Linux next to the runtime.
function(ryn_add_plugin target)
    add_library(${target} SHARED ${ARGN})
    target_link_libraries(${target} PRIVATE RynCommon RynRuntime)
    target_compile_definitions(${target} PRIVATE RYN_BUILDING_PLUGIN)

    # Building a plugin also builds the runtime, so running/debugging a plugin target always has a fresh host
    add_dependencies(${target} RynRuntime)

    if(RYN_PLATFORM_WINDOWS)
        # MinGW would otherwise name it "lib<name>.dll"
        set_target_properties(${target} PROPERTIES PREFIX "")
    endif()
endfunction()
