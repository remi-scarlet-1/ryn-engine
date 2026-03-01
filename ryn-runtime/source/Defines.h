#pragma once

/* Shared Library Management */
#if defined(RYN_PLATFORM_WIN)
    #if defined(RYN_RUNTIME)
        #define RYN_API __declspec(dllexport)
    #else
        #define RYN_API __declspec(dllimport)
    #endif

#elif defined(RYN_PLATFORM_LINUX)
    #if defined(RYN_RUNTIME)
        #define RYN_API __attribute__((visibility("default")))
    #else
        #define RYN_API
    #endif

#else
    #define RYN_API
#endif

/* Configurations & Features */
#if defined(RYN_CONFIG_DEBUG)
    #define RYN_ENABLE_LOGGING
    #define RYN_ENABLE_ASSERTS
    #define RYN_ENABLE_VERIFY

#elif defined(RYN_CONFIG_RELEASE)
    #define RYN_ENABLE_LOGGING
    #define RYN_ENABLE_VERIFY

#elif defined(RYN_CONFIG_DIST)
    #define RYN_ENABLE_VERIFY

#else
    #error "Undefined configuration! Check CMake."
#endif

/* Feature: Logging */
#if defined(RYN_ENABLE_LOGGING)
// clang-format off
    namespace ryn {
        struct DebugScopeTag {};

        namespace {
            template<typename T = void>
            struct DebugScopeName {
                static constexpr auto value = "Unknown";
            };
        }

        template<typename T = void>
        [[maybe_unused]] static consteval const char* resolveDebugScope(DebugScopeTag) { return DebugScopeName<T>::value; }
        [[maybe_unused]] static consteval const char* resolveDebugScope(const char* s) { return s; }
    }

    static constexpr ::ryn::DebugScopeTag s_RynDebugScope;
// clang-format on

    #if defined(RYN_PLATFORM_WIN)
        #define RYN_DISABLE_SHADOW_WARNING __pragma(warning(push)) __pragma(warning(disable : 4459))
        #define RYN_ENABLE_SHADOW_WARNING  __pragma(warning(pop))
    #elif defined(RYN_PLATFORM_LINUX)
        #define RYN_DISABLE_SHADOW_WARNING _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wshadow\"")
        #define RYN_ENABLE_SHADOW_WARNING  _Pragma("GCC diagnostic pop")
    #else
        #define RYN_DISABLE_SHADOW_WARNING
        #define RYN_ENABLE_SHADOW_WARNING
    #endif

    #define RYN_SCOPE_FILE(Name)                                                                                                           \
        namespace ryn {                                                                                                                    \
            namespace {                                                                                                                    \
                template<>                                                                                                                 \
                struct DebugScopeName<void> {                                                                                              \
                    [[maybe_unused]] static constexpr const char* value = Name;                                                            \
                };                                                                                                                         \
            }                                                                                                                              \
        }

    #define RYN_SCOPE_LOCAL(Name)                                                                                                          \
        RYN_DISABLE_SHADOW_WARNING                                                                                                         \
        [[maybe_unused]] static constexpr const char* s_RynDebugScope = Name;                                                              \
        RYN_ENABLE_SHADOW_WARNING

    #include <format>

    #define RYN_TRACE(...)    ::ryn::Log::trace(::ryn::resolveDebugScope(s_RynDebugScope), std::format(__VA_ARGS__).c_str())
    #define RYN_INFO(...)     ::ryn::Log::info(::ryn::resolveDebugScope(s_RynDebugScope), std::format(__VA_ARGS__).c_str())
    #define RYN_WARN(...)     ::ryn::Log::warn(::ryn::resolveDebugScope(s_RynDebugScope), std::format(__VA_ARGS__).c_str())
    #define RYN_ERROR(...)    ::ryn::Log::error(::ryn::resolveDebugScope(s_RynDebugScope), std::format(__VA_ARGS__).c_str())
    #define RYN_CRITICAL(...) ::ryn::Log::critical(::ryn::resolveDebugScope(s_RynDebugScope), std::format(__VA_ARGS__).c_str())
#else
    #define RYN_SCOPE_FILE(Name)
    #define RYN_SCOPE_LOCAL(Name)

    #define RYN_TRACE(...)    (void)0
    #define RYN_INFO(...)     (void)0
    #define RYN_WARN(...)     (void)0
    #define RYN_ERROR(...)    (void)0
    #define RYN_CRITICAL(...) (void)0
#endif

/* Feature: Asserts */
#if defined(RYN_ENABLE_ASSERTS)
    #include <cstdlib>

// clang-format off
    namespace ryn {
        consteval const char* stripFilePath(const char* path) {
            const char* lastSlash = path;
            for (const char* p = path; *p; ++p)
                if (*p == '/' || *p == '\\') lastSlash = p + 1;
            return lastSlash;
        }
    }
// clang-format on

    // Debug break
    #if defined(RYN_PLATFORM_WIN)
        #include <intrin.h>
        #define RYN_DEBUGBREAK() __debugbreak()
    #elif defined(RYN_PLATFORM_LINUX)
        #include <csignal>
        #define RYN_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Undefined debug break on current platform! Check CMake."
    #endif

    // Assert
    #define RYN_ASSERT(Check, ...)                                                                                                         \
        do {                                                                                                                               \
            if (!(Check)) {                                                                                                                \
                RYN_CRITICAL("Assertion Failed" __VA_OPT__(": ") __VA_ARGS__);                                                             \
                RYN_CRITICAL("  at {0}:{1}", ::ryn::stripFilePath(__FILE__), __LINE__);                                                    \
                RYN_DEBUGBREAK();                                                                                                          \
                std::abort();                                                                                                              \
            }                                                                                                                              \
        } while (0)
#else
    #define RYN_ASSERT(Check, ...)
#endif

/* Feature: Verify */
#if defined(RYN_ENABLE_VERIFY)
    #include <cstdlib>

    #define RYN_VERIFY(Check, ...)                                                                                                         \
        do {                                                                                                                               \
            if (!(Check)) {                                                                                                                \
                RYN_CRITICAL("Verify Failed" __VA_OPT__(": ") __VA_ARGS__);                                                                \
                std::abort();                                                                                                              \
            }                                                                                                                              \
        } while (0)
#else
    #define RYN_VERIFY(Check, ...) (void)(Check)
#endif