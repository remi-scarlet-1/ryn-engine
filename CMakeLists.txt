cmake_minimum_required(VERSION 3.28)

# CMAKE... CAN YOU SHUT THE FUCK UP ALREADY
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "" FORCE)

# ===== Project =====
project(RynEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===== Compiler Options =====
if(MSVC)
    add_compile_options(/Zc:preprocessor /Zc:__cplusplus)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

function(ryn_compile_options target)
    if(MSVC) # Strict Warnings
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Werror)
    endif()

    if(MSVC) # Disable RTTI & Exceptions
        target_compile_options(${target} PRIVATE /GR- /EHs-c-)
    else()
        target_compile_options(${target} PRIVATE -fno-rtti -fno-exceptions)
    endif()
endfunction()

# ===== Fetch Dependencies =====
include(dependencies/diligent.cmake)
include(dependencies/entt.cmake)
include(dependencies/glm.cmake)
include(dependencies/imgui.cmake)
include(dependencies/sdl3.cmake)
include(dependencies/spdlog.cmake)

# ===== Subdirectories =====
add_subdirectory(ryn-runtime)
add_subdirectory(ryn-editor)