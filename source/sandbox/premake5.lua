project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    setOutputDirs()
    
    files {
        path.join(dir.source, "sandbox", "premake5.lua"),
        path.join(dir.source, "sandbox", "**.h"),
        path.join(dir.source, "sandbox", "**.hpp"),
        path.join(dir.source, "sandbox", "**.c"),
        path.join(dir.source, "sandbox", "**.cpp"),
    }

    vpaths {
        ["."] = { path.join(dir.source, "sandbox", "premake5.lua") },
        ["headers"] = { 
            path.join(dir.source, "sandbox", "**.h"), 
            path.join(dir.source, "sandbox", "**.hpp") 
        },
        ["sources"] = { 
            path.join(dir.source, "sandbox", "**.c"), 
            path.join(dir.source, "sandbox", "**.cpp") 
        },
    }

    includedirs {
        path.join(dir.source, "runtime", "include"),
        path.join(dir.vendors, "entt", "src"),
        path.join(dir.source, "sandbox"),
    }

    links {
        "Runtime",
        "EnTT",
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

    filter { "action:vs*" }
        system "windows"
        toolset "msc"
        defines {
            "RYN_PLATFORM_WIN",
        }