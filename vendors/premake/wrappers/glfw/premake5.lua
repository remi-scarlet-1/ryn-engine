project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    setOutputDirs()

    files {
        path.join(dir.vendors, "premake", "wrappers", "glfw", "premake5.lua"),

        path.join(dir.vendors, "glfw", "include", "**.h"), 
        path.join(dir.vendors, "glfw", "include", "**.hpp"),

        -- Core
        path.join(dir.vendors, "glfw", "src", "context.c"),
        path.join(dir.vendors, "glfw", "src", "init.c"),
        path.join(dir.vendors, "glfw", "src", "input.c"),
        path.join(dir.vendors, "glfw", "src", "monitor.c"),
        path.join(dir.vendors, "glfw", "src", "vulkan.c"),
        path.join(dir.vendors, "glfw", "src", "window.c"),
        path.join(dir.vendors, "glfw", "src", "platform.c"),

        -- Null backend (needed even if unused)
        path.join(dir.vendors, "glfw", "src", "null_init.c"),
        path.join(dir.vendors, "glfw", "src", "null_monitor.c"),
        path.join(dir.vendors, "glfw", "src", "null_window.c"),
        path.join(dir.vendors, "glfw", "src", "null_joystick.c"),
    }


    vpaths {
        ["./**"] = { "**.lua" },
        ["headers/**"] = { 
            path.join(dir.vendors, "glfw", "include", "**.h"),
            path.join(dir.vendors, "glfw", "include", "**.hpp"),
            path.join(dir.vendors, "glfw", "src", "**.h"), 
            path.join(dir.vendors, "glfw", "src", "**.hpp"),
        },
        ["sources/**"] = { 
            path.join(dir.vendors, "glfw", "src", "**.c"), 
            path.join(dir.vendors, "glfw", "src", "**.cpp") 
        },
    }

    includedirs {
        path.join(dir.vendors, "glfw", "include"),
    }

    filter { "system:windows" }
        systemversion "latest"
        defines { 
            "_GLFW_WIN32",
            "_GLFW_USE_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
        }

        files {
            path.join(dir.vendors, "glfw", "src", "win32_*.*"),
            path.join(dir.vendors, "glfw", "src", "wgl_context.c"),
            path.join(dir.vendors, "glfw", "src", "egl_context.c"),
            path.join(dir.vendors, "glfw", "src", "osmesa_context.c"),
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"