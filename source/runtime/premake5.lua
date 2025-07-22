project "Runtime"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    setOutputDirs()

    files {
        path.join(dir.source, "runtime", "premake5.lua"),
        path.join(dir.source, "runtime", "include", "ryn", "**.h"),
        path.join(dir.source, "runtime", "include", "ryn", "**.hpp"),
        path.join(dir.source, "runtime", "**.c"),
        path.join(dir.source, "runtime", "**.cpp"),
    }

    vpaths {
        ["."] = { path.join(dir.source, "runtime", "premake5.lua") },
        ["headers/**"] = { 
            path.join(dir.source, "runtime", "include", "**.h"), 
            path.join(dir.source, "runtime", "include", "**.hpp") 
        },
        ["sources"] = { 
            path.join(dir.source, "runtime", "**.c"), 
            path.join(dir.source, "runtime", "**.cpp") 
        },
    }

    includedirs {
        path.join(dir.source, "runtime", "include"),
        path.join(dir.vendors, "glfw", "include"),
        path.join(dir.vendors, "imgui"),
        path.join(dir.vendors, "entt", "src"),
    }

    links {
        "GLFW",
        "ImGui",
        "EnTT",
    }

    defines {
        "RYN_RUNTIME",
        "GLFW_EXPOSE_NATIVE_WIN32",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        defines {
            "RYN_CONFIG_DEBUG",
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        defines { 
            "RYN_CONFIG_RELEASE", 
        }

    filter { "system:windows" }
        system "windows"
        toolset "msc"
        defines {
            "RYN_PLATFORM_WIN",
        }