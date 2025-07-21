project "ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    setOutputDirs()

    files {
        path.join(dir.vendors, "premake", "wrappers", "imgui", "premake5.lua"),
        path.join(dir.vendors, "imgui", "*.cpp"),
        path.join(dir.vendors, "imgui", "*.h"),
        path.join(dir.vendors, "imgui", "backends", "imgui_impl_glfw.cpp"),
        path.join(dir.vendors, "imgui", "backends", "imgui_impl_glfw.h"),
        path.join(dir.vendors, "imgui", "backends", "imgui_impl_opengl3.cpp"),
        path.join(dir.vendors, "imgui", "backends", "imgui_impl_opengl3.h"),
    }

    includedirs {
        path.join(dir.vendors, "imgui"),
        path.join(dir.vendors, "glfw", "include"),
    }

    vpaths {
        ["./**"] = { "**.lua" },
        ["headers/**"] = { 
            path.join(dir.vendors, "imgui", "**.h"), 
            path.join(dir.vendors, "imgui", "**.hpp") 
        },
        ["sources/**"] = { 
            path.join(dir.vendors, "imgui", "**.c"), 
            path.join(dir.vendors, "imgui", "**.cpp") 
        },
    }

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"