export module ryn.internal:dynamic_library;

import ryn.core;

export namespace ryn {
    // Longest UTF-8 path the runtime handles, including the terminating null
    inline constexpr usize MAX_PATH_LENGTH = 1024;

    // Owns a loaded shared library (.dll / .so) and unloads it on destruction.
    // TODO(M2): moves into the platform layer next to the file API.
    class DynamicLibrary {
    public:
        DynamicLibrary() = default;
        ~DynamicLibrary();

        DynamicLibrary(const DynamicLibrary&)            = delete;
        DynamicLibrary& operator=(const DynamicLibrary&) = delete;
        DynamicLibrary(DynamicLibrary&& other) noexcept;
        DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

        // `path` is UTF-8. On failure returns false and writes a readable reason into `error`.
        bool load(const char* path, char* error, usize errorCapacity);
        void unload();

        [[nodiscard]] void* findSymbol(const char* name) const;
        [[nodiscard]] bool  isLoaded() const { return m_Handle != nullptr; }

        // "Name" -> "Name.dll" on Windows, "libName.so" on Linux. Returns false if `out` is too small.
        static bool fileName(const char* baseName, char* out, usize capacity);

    private:
        void* m_Handle = nullptr;
    };

    // Writes the UTF-8 folder of the running executable (no trailing separator). Returns false on failure.
    bool executableDirectory(char* out, usize capacity);

    // Path separator of the current platform
    inline constexpr char PATH_SEPARATOR =
#if defined(RYN_PLATFORM_WINDOWS)
        '\\';
#else
        '/';
#endif
}
