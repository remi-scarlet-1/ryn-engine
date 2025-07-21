dir = {}
dir.scripts         = path.getdirectory(_SCRIPT)
dir.root            = path.getabsolute(path.join(dir.scripts, ".."))
dir.source          = path.join(dir.root, "source")
dir.vendors         = path.join(dir.root, "vendors")
dir.generated       = path.join(dir.root, "generated")
dir.builds          = path.join(dir.generated, "builds")

os.chdir(dir.root)

function setOutputDirs()
    targetdir(path.join(dir.builds, "%{cfg.buildcfg}", "bin"))
    objdir   (path.join(dir.builds, "%{cfg.buildcfg}", "obj"))
end

workspace "RynEngine"
    configurations { "Debug", "Release" }
    architecture "x64"
    location(dir.generated)
    startproject "Sandbox"

group "Dependencies"
    include(path.join(dir.vendors, "premake", "wrappers", "entt", "premake5.lua"))
    include(path.join(dir.vendors, "premake", "wrappers", "glfw", "premake5.lua"))
    include(path.join(dir.vendors, "premake", "wrappers", "imgui", "premake5.lua"))

group ""
    include(path.join(dir.source, "runtime", "premake5.lua"))
    include(path.join(dir.source, "sandbox", "premake5.lua"))