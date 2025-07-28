project "Glad"
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    setOutputDirs()

    files {
        path.join(dir.vendors, "premake", "wrappers", "glad", "premake5.lua"),
        path.join(dir.vendors, "glad", "include", "**.h"),
        path.join(dir.vendors, "glad", "include", "**.hpp"),
        path.join(dir.vendors, "glad", "src", "**.c"),
        path.join(dir.vendors, "glad", "src", "**.cpp"),
    }

    includedirs {
        path.join(dir.vendors, "glad", "include"),
    }

    vpaths {
        ["./**"] = { "**.lua" },
        ["headers/**"] = { 
            path.join(dir.vendors, "glad", "include", "**.h"), 
            path.join(dir.vendors, "glad", "include", "**.hpp") 
        },
        ["sources/**"] = { 
            path.join(dir.vendors, "glad", "src", "**.c"), 
            path.join(dir.vendors, "glad", "src", "**.cpp") 
        },
    }

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"