cmake_minimum_required(VERSION 3.28)

project(RynEngine VERSION 0.1 LANGUAGES CXX)

# ===== Language =====
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Reflection (P2996) is only available in GCC 16+ for now
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
    message(FATAL_ERROR
        "Ryn requires GCC 16 or newer for C++26 reflection.\n"
        "Found ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} at: ${CMAKE_CXX_COMPILER}\n"
        "Check that GCC's bin folder comes first on PATH (or in your IDE toolchain), then delete the build folder and reconfigure.")
endif()

# ===== Platform =====
if(WIN32)
    set(RYN_PLATFORM_WINDOWS ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(RYN_PLATFORM_LINUX ON)
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}. Ryn supports Windows x64 and Linux x64.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Ryn only supports 64-bit targets")
endif()

# ===== Output =====
# Runtime and plugins land in one folder so the runtime can find plugins next to itself
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Hide symbols by default; only RYN_API / RYN_PLUGIN_API are exported
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(cmake/Ryn.cmake)

# ===== Projects =====
add_subdirectory(ryn-runtime)
