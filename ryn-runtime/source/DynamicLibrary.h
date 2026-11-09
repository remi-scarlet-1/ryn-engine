#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace ryn {
    // Owns a loaded shared library (.dll / .so) and unloads it on destruction.
    class DynamicLibrary {
    public:
        DynamicLibrary() = default;
        ~DynamicLibrary();

        DynamicLibrary(const DynamicLibrary&)            = delete;
        DynamicLibrary& operator=(const DynamicLibrary&) = delete;
        DynamicLibrary(DynamicLibrary&& other) noexcept;
        DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

        // Returns false and fills `error` on failure
        bool load(const std::filesystem::path& path, std::string& error);
        void unload();

        [[nodiscard]] void* findSymbol(const char* name) const;
        [[nodiscard]] bool  isLoaded() const { return m_Handle != nullptr; }

        // "Name" -> "Name.dll" on Windows, "libName.so" on Linux
        [[nodiscard]] static std::string fileName(std::string_view baseName);

    private:
        void* m_Handle = nullptr;
    };

    // Folder containing the running executable
    [[nodiscard]] std::filesystem::path executableDirectory();
}
