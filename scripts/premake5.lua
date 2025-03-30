-- Construct paths
dir = { script = debug.getinfo(1, "S").source:sub(2):match("(.*/)") } -- Get path of script directory & save into new 'dir' table

dir.script          = dir.script:match("(.*)/$") or dir.script -- Remove trailing slash (if any) from dir.script
dir.root            = path.getabsolute(path.join(dir.script, ".."))
dir.generate        = path.join(dir.root, "generated")

dir.runtime         = path.join(dir.root, "source", "runtime")
dir.runtimeBuild    = path.join(dir.generate, "build")

dir.user            = path.join(dir.root, "source", "user")
dir.userBuild       = path.join(dir.generate, "build")

dir.vendors         = path.join(dir.root, "vendors")
dir.vendorsBuild    = path.join(dir.generate, "vendor_projects")
dir.vendorsInstall  = path.join(dir.generate, "vendor_install")

-- Setup workspace
os.chdir(dir.root)

workspace "RynEngine"  
    configurations { "Release", "Debug" }
    platforms "x64"
    location "%{dir.generate}"

    ---------- Project: Runtime ----------
    project "runtime"
        kind "SharedLib"
        language "C++"
        cppdialect "C++20"
        architecture "x64"

        targetdir "%{dir.runtimeBuild}/bin/%{cfg.platform}/%{cfg.buildcfg}"
        objdir "%{dir.runtimeBuild}/obj/%{cfg.platform}/%{cfg.buildcfg}"

        defines {
            "RYN_EXPORT",
        }

        files { 
            "%{dir.runtime}/**.h", "%{dir.runtime}/**.hpp",
            "%{dir.runtime}/**.c", "%{dir.runtime}/**.cpp",
            "%{dir.vendors}/_header-only/**.c", "%{dir.vendors}/_header-only/**.cpp", -- Include source of some vendors
            "%{dir.vendors}/_header-only/**.h", "%{dir.vendors}/_header-only/**.hpp",
            "%{dir.script}/premake5.lua", "%{dir.root}/docs/**.*", -- Include build scripts & documentation (make viewable in solution explorer)
        }

        includedirs {
            "%{dir.runtime}",
            "%{dir.vendors}/_header-only", -- Include headers of header-only vendors
            "%{dir.vendorsInstall}/include", -- Include headers of linked vendors
        }

        libdirs {
            "%{dir.vendorsInstall}/lib",
        }

        vpaths {
            ["Build"] = { "%{dir.script}/**.*" },
            ["Docs"] = { "%{dir.root}/docs/**.*" },
            ["Headers/*"] = { "%{dir.runtime}/**.h", "%{dir.runtime}/**.hpp" },
            ["Sources"] = { "%{dir.runtime}/**.c", "%{dir.runtime}/**.cpp" },
            ["Vendors (header-only)/*"] = { "%{dir.vendors}/_header-only/**.h", "%{dir.vendors}/_header-only/**.hpp", "%{dir.vendors}/_header-only/**.c", "%{dir.vendors}/_header-only/**.cpp" },
        }

        ----- Configuration filters -----
        filter "configurations:Release"
            optimize "On"
            defines { 
                "RYN_CONFIG_RELEASE", 
                "BX_CONFIG_DEBUG=0" 
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
                "GLFW_EXPOSE_NATIVE_WIN32", -- required for glfw (windows)
            }

            buildoptions { 
                "/Zc:__cplusplus", -- required for bgfx (msvc)
                "/Zc:preprocessor", -- required for bgfx (msvc)
            }

            includedirs { 
                "%{dir.vendorsInstall}/include/bx/compat/msvc",  -- required for bgfx (msvc)
            }

            links {
                "Ws2_32",
            }

    ---------- Project: Unit Testing ----------
    project "unit_tests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        architecture "x64"

        

    --[[
    project "user"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        architecture "x64"

        targetdir "%{dir.userBuild}/bin/%{cfg.platform}/%{cfg.buildcfg}"
        objdir "%{dir.userBuild}/obj/%{cfg.platform}/%{cfg.buildcfg}"

        files { 
            "%{dir.user}/**.h", "%{dir.user}/**.hpp",
            "%{dir.user}/**.c", "%{dir.user}/**.cpp",
        }

        includedirs {
            "%{dir.runtime}",
            "%{dir.runtime}/public", -- public engine headers
        }

        libdirs {
            "%{dir.runtimeBuild}/bin/%{cfg.buildcfg}", -- Where the runtime .lib is output
            "%{dir.vendorsInstall}/lib",
        }

        links {
            "runtime", -- Link against the runtime shared library
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
        ]]--

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
function clean()
    os.rmdir(dir.generate)
end

function cleanVendors()
    os.rmdir(dir.vendorsBuild)
    os.rmdir(dir.vendorsInstall)
end

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
    trigger = "clean",
    description = "Remove all generated project files, including project files and artifacts of vendors",
    execute = clean
}

newaction {
    trigger = "clean-vendors",
    description = "Remove generated project files and artifacts of vendors",
    execute = cleanVendors
}

newaction {
    trigger = "vs2022-vendors",
    description = "Generate Visual Studio 2022 project files of vendors, build and install",
    execute = buildVendors
}