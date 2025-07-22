project "EnTT"
    kind "Utility"
    language "C++"
    staticruntime "off"

    setOutputDirs()

    files {
        path.join(dir.vendors, "premake", "wrappers", "entt", "premake5.lua"),
        path.join(dir.vendors, "entt", "src", "entt", "**.h"), 
        path.join(dir.vendors, "entt", "src", "entt", "**.hpp"),
    }

    vpaths {
        ["./**"] = { "**.lua" },
        ["headers/**"] =  { 
            path.join(dir.vendors, "entt", "src", "entt", "**.h"), 
            path.join(dir.vendors, "entt", "src", "entt", "**.hpp"), 
        }
    }

    includedirs {
        path.join(dir.vendors, "entt", "src"),
    }