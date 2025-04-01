-- Construct important paths
dir = { scripts = debug.getinfo(1, "S").source:sub(2):match("(.*/)") } -- Get path of script directory & save into new 'dir' table
dir.scripts         = dir.scripts:match("(.*)/$") or dir.scripts -- Remove trailing slash (if any) from dir.script

dir.root            = path.getabsolute(path.join(dir.scripts, ".."))
dir.docs            = path.join(dir.root, "docs")
dir.projects        = path.join(dir.root, "projects")
dir.vendors         = path.join(dir.root, "vendors")

dir.generated       = path.join(dir.root, "generated")
dir.builds          = path.join(dir.generated, "builds")
dir.vendorArtifacts = path.join(dir.generated, "vendorArtifacts")
dir.vendorBuilds    = path.join(dir.generated, "vendorBuilds")

dir.sourceRuntime   = path.join(dir.projects, "runtime")
dir.sourceTests     = path.join(dir.projects, "tests")

-- Set working directory to root
os.chdir(dir.root)

-- Include other scripts
dofile("PremakeHelpers.lua")
dofile("PremakeVendors.lua")

-- Premake workspace
workspace "RynEngine"  
    configurations { "Release", "Debug" }
    platforms "x64"
    location "%{dir.generated}"

    --------------------------------------------------------------------------------
    ------------------------------- [ Project: All ] -------------------------------
    --------------------------------------------------------------------------------
    project "all"
        kind "None"
        language "C++"

        targetdir   "%{dir.builds}/%{cfg.buildcfg}/%{cfg.platform}/bin"
        objdir      "%{dir.builds}/%{cfg.buildcfg}/%{cfg.platform}/obj"

        files {
            "%{dir.docs}/**.*",
            "%{dir.scripts}/**.*", 
        }

        vpaths {
            ["Docs"] = { "%{dir.docs}/**.*" },
            ["Scripts"] = { "%{dir.scripts}/**.*" },
        }

    --------------------------------------------------------------------------------
    ----------------------------- [ Project: Runtime ] -----------------------------
    --------------------------------------------------------------------------------
    project "runtime"
        kind "SharedLib"
        language "C++"
        cppdialect "C++20"
        architecture "x64"

        targetdir   "%{dir.builds}/%{cfg.buildcfg}/%{cfg.platform}/bin"
        objdir      "%{dir.builds}/%{cfg.buildcfg}/%{cfg.platform}/obj"

        defines {
            "RYN_EXPORT",
        }
        
        vendorLibs = {
            "%{dir.vendorArtifacts}/bgfx.cmake/lib/",
            "%{dir.vendorArtifacts}/glfw/lib/",
        }

        vendorSource = {
            "%{dir.vendorArtifacts}/imgui/source/**.*",
            "%{dir.vendorArtifacts}/entt/source/**.*",
        }

        vendorInclude = {
            "%{dir.vendorArtifacts}/bgfx.cmake/include/",
            "%{dir.vendorArtifacts}/glfw/include/",
            "%{dir.vendorArtifacts}/imgui/include/",
            "%{dir.vendorArtifacts}/entt/include/",
        }

        files {
            "%{dir.sourceRuntime}/**.h", "%{dir.sourceRuntime}/**.hpp",
            "%{dir.sourceRuntime}/**.c", "%{dir.sourceRuntime}/**.cpp",
            table.unpack(vendorSource),
        }

        vpaths {
            ["Headers"] = { "%{dir.sourceRuntime}/**.h", "%{dir.sourceRuntime}/**.hpp" },
            ["Sources"] = { "%{dir.sourceRuntime}/**.c", "%{dir.sourceRuntime}/**.cpp" },
            ["Vendor by Source (Read-only)/*"] = { table.unpack(vendorSource) },
        }
        
        includedirs { 
            "%{dir.sourceRuntime}/",
            table.unpack(vendorInclude),
        }

        libdirs(vendorLibs)

        ----- Configuration filters -----
        filter "configurations:Release"
            optimize "On"
            defines { 
                "RYN_CONFIG_RELEASE", 
                "BX_CONFIG_DEBUG=0",
            }

            links {
                "bgfx", "bx",
                "bimg", "bimg_decode", "bimg_encode",
                "glfw3",
            }

        filter "configurations:Debug"
            symbols "On"
            defines { 
                "RYN_CONFIG_DEBUG", 
                "BX_CONFIG_DEBUG=1" 
            }

            links {
                "bgfx_d", "bx_d", 
                "bimg_d", "bimg_decode_d", "bimg_encode_d",
                "glfw3_d",
            }

        ----- System filters -----
        filter { "system:windows", "action:vs*" } -- windows (msvc)
            system "windows"
            toolset "msc"

            defines {
                "RYN_PLATFORM_WIN",
                "GLFW_EXPOSE_NATIVE_WIN32", -- required by glfw (windows)
            }

            buildoptions { 
                "/Zc:__cplusplus",  -- required by bgfx (msvc)
                "/Zc:preprocessor", -- required by bgfx (msvc)
            }

            includedirs { 
                "%{dir.vendorsInstall}/include/bx/compat/msvc", -- required by bgfx (msvc)
            }

            links {
                "Ws2_32", -- native windows networking
            }

    --------------------------------------------------------------------------------
    --------------------------- [ Project: (Unit) Test ] ---------------------------
    --------------------------------------------------------------------------------
    -- project "tests"
    --     kind "ConsoleApp"
    --     language "C++"
    --     cppdialect "C++20"
    --     architecture "x64"

    --     targetdir   "%{dir.builds}/%{cfg.buildcfg}%{cfg.platform}/bin"
    --     objdir      "%{dir.builds}/%{cfg.buildcfg}%{cfg.platform}/obj"

    --     files { 
    --         "%{dir.testsSource}/**.h", "%{dir.testsSource}/**.hpp",
    --         "%{dir.testsSource}/**.c", "%{dir.testsSource}/**.cpp",
    --     }

    --     vpaths {
    --         ["Headers"] = { "%{dir.testsSource}/**.h", "%{dir.testsSource}/**.hpp" },
    --         ["Sources"] = { "%{dir.testsSource}/**.c", "%{dir.testsSource}/**.cpp" },
    --     }

    --     includedirs {
    --         "%{dir.testsSource}",
    --         "%{dir.runtimeSource}",
    --     }

    --     libdirs {

    --     }

    --     links {
    --         "runtime",
    --     }

    --     ----- Configuration filters -----
    --     filter "configurations:Release"
    --         optimize "On"
    --         defines { "RYN_CONFIG_RELEASE" }

    --     filter "configurations:Debug"
    --         symbols "On"
    --         defines { "RYN_CONFIG_DEBUG" }

    --     ----- System filters -----
    --     filter { "system:windows", "action:vs*" } -- windows (msvc)
    --         system "windows"
    --         toolset "msc"

    --         defines { "RYN_PLATFORM_WIN" }