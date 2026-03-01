include(FetchContent)

set(DILIGENT_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(DILIGENT_NO_VULKAN OFF CACHE BOOL "" FORCE)
set(DILIGENT_NO_OPENGL ON CACHE BOOL "" FORCE)
set(DILIGENT_NO_METAL ON CACHE BOOL "" FORCE)
set(DILIGENT_NO_WEBGPU ON CACHE BOOL "" FORCE)
set(DILIGENT_NO_DIRECT3D12 ON CACHE BOOL "" FORCE)
set(DILIGENT_NO_DIRECT3D11 ON CACHE BOOL "" FORCE)

if(WIN32) # Enable D3D on Windows
    set(DILIGENT_NO_DIRECT3D12 OFF CACHE BOOL "" FORCE)
    set(DILIGENT_NO_DIRECT3D11 OFF CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(DiligentCore
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentCore.git
    GIT_TAG 08c465f
    SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/DiligentCore"
    UPDATE_DISCONNECTED ON
)

FetchContent_Declare(DiligentTools
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentTools.git
    GIT_TAG 560e5c0
    SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/DiligentTools"
    UPDATE_DISCONNECTED ON
)

FetchContent_Declare(DiligentFX
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentFX.git
    GIT_TAG 290f0cf
    SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/DiligentFX"
    UPDATE_DISCONNECTED ON
)

FetchContent_MakeAvailable(DiligentCore DiligentTools DiligentFX)

# Patch Diligent by stripping usage of exceptions for std::terminate()
set(DILIGENT_ERRORS_HPP "${diligentcore_SOURCE_DIR}/Primitives/interface/Errors.hpp")

if(EXISTS "${DILIGENT_ERRORS_HPP}")
    message(STATUS "Patching Diligent: Removing exceptions from ${DILIGENT_ERRORS_HPP}")
    file(READ "${DILIGENT_ERRORS_HPP}" DILIGENT_ERRORS_CONTENT)

    if(NOT DILIGENT_ERRORS_CONTENT MATCHES "// RynPatched")
        string(REPLACE "throw std::runtime_error(std::move(msg));" "std::terminate();" DILIGENT_ERRORS_CONTENT "${DILIGENT_ERRORS_CONTENT}")
        string(PREPEND DILIGENT_ERRORS_CONTENT "// RynPatched\n")
        file(WRITE "${DILIGENT_ERRORS_HPP}" "${DILIGENT_ERRORS_CONTENT}")
    endif()
endif()

# Patch Dilignt by stripping usage of RTTI
set(DILIGENT_DEBUG_UTILS_HPP "${diligentcore_SOURCE_DIR}/Platforms/Basic/interface/DebugUtilities.hpp")

if(EXISTS "${DILIGENT_DEBUG_UTILS_HPP}")
    message(STATUS "Patching Diligent: Removing RTTI from ${DILIGENT_DEBUG_UTILS_HPP}")
    file(READ "${DILIGENT_DEBUG_UTILS_HPP}" DILIGENT_DEBUG_UTILS_CONTENT)

    if(NOT DILIGENT_DEBUG_UTILS_CONTENT MATCHES "// RynPatched")
        string(REPLACE [[#    include <typeinfo>]] [[// RTTI Disabled]] DILIGENT_DEBUG_UTILS_CONTENT "${DILIGENT_DEBUG_UTILS_CONTENT}")
        string(REPLACE [[CheckDynamicType<DstType>(pSrcPtr);]] [[]] DILIGENT_DEBUG_UTILS_CONTENT "${DILIGENT_DEBUG_UTILS_CONTENT}")
        string(REGEX REPLACE
            [[void CheckDynamicType\(SrcType\* pSrcPtr\)[^{]*\{[^}]*\}]]
            [[void CheckDynamicType(SrcType* pSrcPtr) {}]]
            DILIGENT_DEBUG_UTILS_CONTENT "${DILIGENT_DEBUG_UTILS_CONTENT}")
        string(PREPEND DILIGENT_DEBUG_UTILS_CONTENT "// RynPatched\n")
        file(WRITE "${DILIGENT_DEBUG_UTILS_HPP}" "${DILIGENT_DEBUG_UTILS_CONTENT}")
    endif()
endif()