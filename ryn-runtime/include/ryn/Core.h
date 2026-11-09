#pragma once

// RYN_API marks functions the runtime exports to plugins.
// RYN_PLUGIN_API marks the entry point a plugin exports to the runtime.
#if defined(RYN_PLATFORM_WINDOWS)
    #define RYN_EXPORT __declspec(dllexport)
    #define RYN_IMPORT __declspec(dllimport)
#elif defined(RYN_PLATFORM_LINUX)
    #define RYN_EXPORT __attribute__((visibility("default")))
    #define RYN_IMPORT __attribute__((visibility("default")))
#else
    #error "Unknown platform. Check CMake."
#endif

#if defined(RYN_BUILDING_RUNTIME)
    #define RYN_API RYN_EXPORT
#else
    #define RYN_API RYN_IMPORT
#endif

#define RYN_PLUGIN_API extern "C" RYN_EXPORT

#if !defined(RYN_CONFIG_DEBUG) && !defined(RYN_CONFIG_RELEASE) && !defined(RYN_CONFIG_DIST)
    #error "Unknown configuration. Check CMake."
#endif
