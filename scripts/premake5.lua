-- Construct paths
dir = { scripts = debug.getinfo(1, "S").source:sub(2):match("(.*/)") } -- Get path of script directory & save into new 'dir' table
dir.scripts          = dir.scripts:match("(.*)/$") or dir.scripts -- Remove trailing slash (if any) from dir.script

dir.root            = path.getabsolute(path.join(dir.scripts, ".."))
dir.docs            = path.join(dir.root, "docs")
dir.generated       = path.join(dir.root, "generated")
dir.source          = path.join(dir.root, "source")
dir.vendors         = path.join(dir.root, "vendors")

dir.builds          = path.join(dir.generated, "builds")
dir.vendorsBuild    = path.join(dir.generated, "vendorBuilds")
dir.vendorsInstall  = path.join(dir.generated, "vendorInstall")
dir.vendorsBySource = path.join(dir.vendors, "_bysource")

dir.runtimeSource   = path.join(dir.source, "runtime")
dir.testsSource     = path.join(dir.source, "tests")

-- Setup workspace
os.chdir(dir.root)

workspace "RynEngine"  
    configurations { "Release", "Debug" }
    platforms "x64"
    location "%{dir.generated}"

    --------------- [ Project: Runtime ] ---------------
    project "runtime"
        kind "SharedLib"
        language "C++"
        cppdialect "C++20"
        architecture "x64"

        targetdir   "%{dir.builds}/%{cfg.buildcfg}%{cfg.platform}/bin"
        objdir      "%{dir.builds}/%{cfg.buildcfg}%{cfg.platform}/obj"

        defines {
            "RYN_EXPORT",
        }

        files { 
            "%{dir.runtimeSource}/**.h", "%{dir.runtimeSource}/**.hpp",
            "%{dir.runtimeSource}/**.c", "%{dir.runtimeSource}/**.cpp",
            -- Add some vendors by source
            "%{dir.vendorsBySource}/**.h", "%{dir.vendorsBySource}/**.hpp",
            "%{dir.vendorsBySource}/**.c", "%{dir.vendorsBySource}/**.cpp",
            -- Add scripts & docs (make viewable in solution explorer)
            "%{dir.scripts}/**.lua", 
            "%{dir.docs}/**.*",
        }

        vpaths {
            ["Docs"] = { "%{dir.docs}/**.*" },
            ["Scripts"] = { "%{dir.scripts}/**.lua" },
            ["Headers"] = { "%{dir.runtimeSource}/**.h", "%{dir.runtimeSource}/**.hpp" },
            ["Sources"] = { "%{dir.runtimeSource}/**.c", "%{dir.runtimeSource}/**.cpp" },
            ["Vendors (by source)/*"] = { 
                "%{dir.vendorsBySource}/**.h", "%{dir.vendorsBySource}/**.hpp", 
                "%{dir.vendorsBySource}/**.c", "%{dir.vendorsBySource}/**.cpp" 
            },
        }

        includedirs {
            "%{dir.runtimeSource}",
            "%{dir.vendorsBySource}",           -- Include headers of vendors added by source
            "%{dir.vendorsInstall}/include",    -- Include headers of vendors added by building & linking
        }

        libdirs {
            "%{dir.vendorsInstall}/lib",
        }

        ----- Configuration filters -----
        filter "configurations:Release"
            optimize "On"
            defines { 
                "RYN_CONFIG_RELEASE", 
                "BX_CONFIG_DEBUG=0",
            }

            links {
                "bgfx", 
                "bimg", 
                "bimg_decode", 
                "bimg_encode", 
                "bx",
                "glfw3",
            }

        filter "configurations:Debug"
            symbols "On"
            defines { 
                "RYN_CONFIG_DEBUG", 
                "BX_CONFIG_DEBUG=1" 
            }

            links {
                "bgfx_d", 
                "bimg_d", 
                "bimg_decode_d", 
                "bimg_encode_d", 
                "bx_d",
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

    --------------- [ Project: (Unit) Tests ] ---------------
    project "tests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        architecture "x64"

        targetdir   "%{dir.builds}/%{cfg.buildcfg}%{cfg.platform}/bin"
        objdir      "%{dir.builds}/%{cfg.buildcfg}%{cfg.platform}/obj"

        files { 
            "%{dir.testsSource}/**.h", "%{dir.testsSource}/**.hpp",
            "%{dir.testsSource}/**.c", "%{dir.testsSource}/**.cpp",
        }

        vpaths {
            ["Headers"] = { "%{dir.testsSource}/**.h", "%{dir.testsSource}/**.hpp" },
            ["Sources"] = { "%{dir.testsSource}/**.c", "%{dir.testsSource}/**.cpp" },
        }

        includedirs {
            "%{dir.testsSource}",
            "%{dir.runtimeSource}",
        }

        libdirs {

        }

        links {
            "runtime",
        }

        ----- Configuration filters -----
        filter "configurations:Release"
            optimize "On"
            defines { "RYN_CONFIG_RELEASE" }

        filter "configurations:Debug"
            symbols "On"
            defines { "RYN_CONFIG_DEBUG" }

        ----- System filters -----
        filter { "system:windows", "action:vs*" } -- windows (msvc)
            system "windows"
            toolset "msc"

            defines { "RYN_PLATFORM_WIN" }



-- Helper functions
local function touchDir(dir)
    if not os.isdir(dir) then os.mkdir(dir) end
end

local function safeExecute(command)
    print("Executing: " .. command)
    local result = os.execute(command)
    if not result then error("Command failed: " .. command) end
end

local function cmakeGenerate(sourceDir, buildDir, installDir, generator, options)
    local command = string.format(
        'cmake -S "%s" -B "%s" -D CMAKE_INSTALL_PREFIX="%s" -G "%s" %s -D CMAKE_RELEASE_POSTFIX="" -D CMAKE_DEBUG_POSTFIX="_d" -D CMAKE_CXX_FLAGS_INIT="/w" -D CMAKE_C_FLAGS_INIT="/w"',
        sourceDir, buildDir, installDir, generator, options or "")
    safeExecute(command)
end

local function cmakeBuildAndInstall(buildDir)
    local processors = os.getenv("NUMBER_OF_PROCESSORS") or 4

    local buildDebugCommand = string.format('cmake --build "%s" --config Debug --parallel %d', buildDir, processors)
    local buildReleaseCommand = string.format('cmake --build "%s" --config Release --parallel %d', buildDir, processors)
    safeExecute(buildDebugCommand)
    safeExecute(buildReleaseCommand)

    local installDebugCommand = string.format('cmake --install "%s" --config Debug', buildDir)
    local installReleaseCommand = string.format('cmake --install "%s" --config Release', buildDir)
    safeExecute(installDebugCommand)
    safeExecute(installReleaseCommand)
end

-- Action functions
function buildVendors()
    local generator = "Visual Studio 17 2022" -- TODO: make this configurable

    touchDir(dir.vendorsBuild)
    touchDir(dir.vendorsInstall)

    -- Build BGFX
    cmakeGenerate(path.join(dir.vendors, "bgfx.cmake"), path.join(dir.vendorsBuild, "bgfx.cmake"), dir.vendorsInstall, generator, "")
    cmakeBuildAndInstall(path.join(dir.vendorsBuild, "bgfx.cmake"))
    
    -- Build GLFW
    cmakeGenerate(path.join(dir.vendors, "glfw"), path.join(dir.vendorsBuild, "glfw"), dir.vendorsInstall, generator, "-D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF")
    cmakeBuildAndInstall(path.join(dir.vendorsBuild, "glfw"))
end

-- Actions
newaction {
    trigger = "vs2022-vendors",
    description = "Generate Visual Studio 2022 project files of vendors, build and install",
    execute = buildVendors
}