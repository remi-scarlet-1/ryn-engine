#include "DynamicLibrary.h"

#include <utility>

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
#endif

namespace ryn {
    DynamicLibrary::~DynamicLibrary() { unload(); }

    DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept : m_Handle(std::exchange(other.m_Handle, nullptr)) {}

    DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept {
        if (this != &other) {
            unload();
            m_Handle = std::exchange(other.m_Handle, nullptr);
        }
        return *this;
    }

#if defined(RYN_PLATFORM_WINDOWS)
    bool DynamicLibrary::load(const std::filesystem::path& path, std::string& error) {
        unload();
        m_Handle = ::LoadLibraryW(path.c_str());
        if (!m_Handle) {
            error = std::system_category().message(static_cast<int>(::GetLastError()));
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

    std::string DynamicLibrary::fileName(std::string_view baseName) { return std::string(baseName) + ".dll"; }

    std::filesystem::path executableDirectory() {
        std::wstring buffer(MAX_PATH, L'\0');
        while (true) {
            const DWORD length = ::GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
            if (length < buffer.size()) {
                buffer.resize(length);
                break;
            }
            buffer.resize(buffer.size() * 2);
        }
        return std::filesystem::path(buffer).parent_path();
    }

#elif defined(RYN_PLATFORM_LINUX)
    bool DynamicLibrary::load(const std::filesystem::path& path, std::string& error) {
        unload();
        m_Handle = ::dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!m_Handle) {
            const char* message = ::dlerror();
            error               = message ? message : "unknown dlopen error";
            return false;
        }
        return true;
    }

    void DynamicLibrary::unload() {
        if (m_Handle) ::dlclose(m_Handle);
        m_Handle = nullptr;
    }

    void* DynamicLibrary::findSymbol(const char* name) const { return m_Handle ? ::dlsym(m_Handle, name) : nullptr; }

    std::string DynamicLibrary::fileName(std::string_view baseName) { return "lib" + std::string(baseName) + ".so"; }

    std::filesystem::path executableDirectory() {
        std::error_code ec;
        return std::filesystem::read_symlink("/proc/self/exe", ec).parent_path();
    }
#endif
}
