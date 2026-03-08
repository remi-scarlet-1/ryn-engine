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
    #error "Unknown platform. Shared library exports are undefined for this platform."
#endif

/* Configurations & Features */
#if defined(RYN_CONFIG_DEBUG)
    #define RYN_ENABLE_LOGGING
    #define RYN_ENABLE_VERIFY
    #define RYN_ENABLE_ASSERTS

#elif defined(RYN_CONFIG_RELEASE)
    #define RYN_ENABLE_LOGGING
    #define RYN_ENABLE_VERIFY

#elif defined(RYN_CONFIG_DIST)
    #define RYN_ENABLE_VERIFY

#else
    #error "Unknown configuration. Check CMake."
#endif

/* Feature: Logging */
#if defined(RYN_ENABLE_LOGGING)
// clang-format off
    namespace ryn::debug {
        namespace {
            template<typename T = void>
            struct ScopeFileName {
                static constexpr auto value = "Unknown";
            };
        }

        struct ScopeLocalTag {};

        template<typename T = void>
        [[maybe_unused]] static consteval const char* resolveScopeFile() {
            return ScopeFileName<T>::value;
        }
        [[maybe_unused]] static consteval const char* resolveScopeLocal(ScopeLocalTag) { return nullptr; }
        [[maybe_unused]] static consteval const char* resolveScopeLocal(const char* s) { return s; }
    }

    static constexpr ::ryn::debug::ScopeLocalTag s_RynScopeLocal;
// clang-format on

    #if defined(_MSC_VER)
        #define RYN_COMPILER_WARNING_DISABLE_SHADOW __pragma(warning(push)) __pragma(warning(disable : 4459))
        #define RYN_COMPILER_WARNING_ENABLE_SHADOW  __pragma(warning(pop))
    #elif defined(__GNUC__) || defined(__clang__)
        #define RYN_COMPILER_WARNING_DISABLE_SHADOW _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wshadow\"")
        #define RYN_COMPILER_WARNING_ENABLE_SHADOW  _Pragma("GCC diagnostic pop")
    #else
        #define RYN_COMPILER_WARNING_DISABLE_SHADOW
        #define RYN_COMPILER_WARNING_ENABLE_SHADOW
    #endif

    #define RYN_SCOPE_FILE(Name)                                                                                                           \
        namespace ryn::debug {                                                                                                             \
            namespace {                                                                                                                    \
                template<>                                                                                                                 \
                struct ScopeFileName<void> {                                                                                               \
                    [[maybe_unused]] static constexpr const char* value = Name;                                                            \
                };                                                                                                                         \
            }                                                                                                                              \
        }

    #define RYN_SCOPE_LOCAL(Name)                                                                                                          \
        RYN_COMPILER_WARNING_DISABLE_SHADOW                                                                                                \
        [[maybe_unused]] static constexpr const char* s_RynScopeLocal = Name;                                                              \
        RYN_COMPILER_WARNING_ENABLE_SHADOW

    #define RYN_LOG(Level, Fmt, ...)                                                                                                       \
        do {                                                                                                                               \
            static constexpr const char* primary   = ::ryn::debug::resolveScopeFile();                                                     \
            static constexpr const char* secondary = ::ryn::debug::resolveScopeLocal(s_RynScopeLocal);                                     \
            if constexpr (secondary == nullptr) {                                                                                          \
                ::ryn::Logger::message(Level, primary, Fmt __VA_OPT__(, ) __VA_ARGS__);                                                    \
            } else {                                                                                                                       \
                ::ryn::Logger::messageLocal(Level, primary, secondary, Fmt __VA_OPT__(, ) __VA_ARGS__);                                    \
            }                                                                                                                              \
        } while (0)

    #define RYN_TRACE(Fmt, ...)    RYN_LOG(ryn::LogLevel::Trace, Fmt __VA_OPT__(, ) __VA_ARGS__)
    #define RYN_INFO(Fmt, ...)     RYN_LOG(ryn::LogLevel::Info, Fmt __VA_OPT__(, ) __VA_ARGS__)
    #define RYN_WARN(Fmt, ...)     RYN_LOG(ryn::LogLevel::Warn, Fmt __VA_OPT__(, ) __VA_ARGS__)
    #define RYN_ERROR(Fmt, ...)    RYN_LOG(ryn::LogLevel::Error, Fmt __VA_OPT__(, ) __VA_ARGS__)
    #define RYN_CRITICAL(Fmt, ...) RYN_LOG(ryn::LogLevel::Critical, Fmt __VA_OPT__(, ) __VA_ARGS__)
#else
    #define RYN_SCOPE_FILE(Name)
    #define RYN_SCOPE_LOCAL(Name)

    #define RYN_TRACE(Fmt, ...)    (void)0
    #define RYN_INFO(Fmt, ...)     (void)0
    #define RYN_WARN(Fmt, ...)     (void)0
    #define RYN_ERROR(Fmt, ...)    (void)0
    #define RYN_CRITICAL(Fmt, ...) (void)0
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

/* Other Macros */
#define RYN_CONCAT_DEFINE_IMPL(a, b) a##b
#define RYN_CONCAT_DEFINE(a, b)      RYN_CONCAT_DEFINE_IMPL(a, b)

#define RYN_KEEP_REFLECTED_TYPE(Type)                                                                                                      \
    static_assert(::ryn::IsType<Type>,                                                                                                     \
                  "REFLECTION ERROR: " #Type " is not TypeReflectable. "                                                                   \
                  "Ensure it inherits either from ryn::StaticType or ryn::DynamicType");                                                   \
    namespace {                                                                                                                            \
        static const bool RYN_CONCAT_DEFINE(ryn_type_reflection_keep_, __LINE__) = Type::registerType();                                   \
    }