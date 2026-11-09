module;
#include <cstdio>
#include <cstring>

#if defined(RYN_PLATFORM_WINDOWS)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX // MinGW's libstdc++ already defines it
        #define NOMINMAX
    #endif
    #include <windows.h>
#elif defined(RYN_PLATFORM_LINUX)
    #include <dlfcn.h>
    #include <unistd.h>
#endif

module ryn.internal;

namespace ryn {
    namespace {
        // Copies `text` into `out`, truncating if needed; always null-terminates when capacity > 0
        void copyString(const char* text, char* out, usize capacity) {
            if (capacity == 0) return;
            std::snprintf(out, capacity, "%s", text ? text : "");
        }
    }

    DynamicLibrary::~DynamicLibrary() { unload(); }

    DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept : m_Handle(other.m_Handle) { other.m_Handle = nullptr; }

    DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept {
        if (this != &other) {
            unload();
            m_Handle       = other.m_Handle;
            other.m_Handle = nullptr;
        }
        return *this;
    }

#if defined(RYN_PLATFORM_WINDOWS)
    bool DynamicLibrary::load(const char* path, char* error, usize errorCapacity) {
        unload();

        // Win32 wants UTF-16 paths
        wchar_t widePath[MAX_PATH_LENGTH];
        if (::MultiByteToWideChar(CP_UTF8, 0, path, -1, widePath, static_cast<int>(MAX_PATH_LENGTH)) == 0) {
            copyString("path is not valid UTF-8 or too long", error, errorCapacity);
            return false;
        }

        m_Handle = ::LoadLibraryW(widePath);
        if (!m_Handle) {
            if (errorCapacity == 0) return false;
            const DWORD length = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                                  nullptr,
                                                  ::GetLastError(),
                                                  0,
                                                  error,
                                                  static_cast<DWORD>(errorCapacity),
                                                  nullptr);
            // Strip the trailing "\r\n" Windows appends
            usize end = length;
            while (end > 0 && (error[end - 1] == '\r' || error[end - 1] == '\n' || error[end - 1] == ' '))
                --end;
            error[end] = '\0';
            if (length == 0) copyString("unknown LoadLibrary error", error, errorCapacity);
            return false;
        }
        return true;
    }

    void DynamicLibrary::unload() {
        if (m_Handle) ::FreeLibrary(static_cast<HMODULE>(m_Handle));
        m_Handle = nullptr;
    }

    void* DynamicLibrary::findSymbol(const char* name) const {
        return m_Handle ? reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(m_Handle), name)) : nullptr;
    }

    bool DynamicLibrary::fileName(const char* baseName, char* out, usize capacity) {
        const int written = std::snprintf(out, capacity, "%s.dll", baseName);
        return written > 0 && static_cast<usize>(written) < capacity;
    }

    bool executableDirectory(char* out, usize capacity) {
        wchar_t     widePath[MAX_PATH_LENGTH];
        const DWORD length = ::GetModuleFileNameW(nullptr, widePath, static_cast<DWORD>(MAX_PATH_LENGTH));
        if (length == 0 || length >= MAX_PATH_LENGTH) return false;

        const int written = ::WideCharToMultiByte(CP_UTF8, 0, widePath, -1, out, static_cast<int>(capacity), nullptr, nullptr);
        if (written == 0) return false;

        // Cut off the executable's file name
        char* lastSeparator = std::strrchr(out, '\\');
        if (!lastSeparator) return false;
        *lastSeparator = '\0';
        return true;
    }

#elif defined(RYN_PLATFORM_LINUX)
    bool DynamicLibrary::load(const char* path, char* error, usize errorCapacity) {
        unload();
        m_Handle = ::dlopen(path, RTLD_NOW | RTLD_LOCAL);
        if (!m_Handle) {
            const char* message = ::dlerror();
            copyString(message ? message : "unknown dlopen error", error, errorCapacity);
            return false;
        }
        return true;
    }

    void DynamicLibrary::unload() {
        if (m_Handle) ::dlclose(m_Handle);
        m_Handle = nullptr;
    }

    void* DynamicLibrary::findSymbol(const char* name) const { return m_Handle ? ::dlsym(m_Handle, name) : nullptr; }

    bool DynamicLibrary::fileName(const char* baseName, char* out, usize capacity) {
        const int written = std::snprintf(out, capacity, "lib%s.so", baseName);
        return written > 0 && static_cast<usize>(written) < capacity;
    }

    bool executableDirectory(char* out, usize capacity) {
        if (capacity == 0) return false;
        const ssize_t length = ::readlink("/proc/self/exe", out, capacity - 1);
        if (length <= 0) return false;
        out[length] = '\0';

        char* lastSeparator = std::strrchr(out, '/');
        if (!lastSeparator) return false;
        *lastSeparator = '\0';
        return true;
    }
#endif
}
