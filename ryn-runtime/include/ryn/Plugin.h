#pragma once

#include <new>

#include "ryn/Defines.h"

/**
 * @brief   Exports a plugin to the runtime.
 * @details Use once per plugin, in a .cpp that imports ryn. Defines the PLUGIN_GET_DESC_SYMBOL function.
 * @param   RootType    Root type, must satisfy ryn::IsPluginRoot.
 * @param   DisplayName Plugin name as a string literal.
 */
#define RYN_PLUGIN(RootType, DisplayName)                                                                              \
    static_assert(::ryn::IsPluginRoot<RootType>,                                                                       \
                  #RootType " must be default constructible, have onLoad() and onUnload(), and no virtual functions"); \
    extern "C" RYN_EXPORT const ::ryn::PluginDesc* rynPluginGetDesc() {                                                \
        static constexpr ::ryn::PluginDesc desc{                                                                       \
            .AbiVersion  = ::ryn::PLUGIN_ABI_VERSION,                                                                  \
            .Name        = DisplayName,                                                                                \
            .RootSize    = sizeof(RootType),                                                                           \
            .RootAlign   = alignof(RootType),                                                                          \
            .FnConstruct = [](void* memory) { ::new (memory) RootType(); },                                            \
            .FnDestruct  = [](void* root) { static_cast<RootType*>(root)->~RootType(); },                              \
            .OnLoad      = [](void* root) { static_cast<RootType*>(root)->onLoad(); },                                 \
            .OnUnload    = [](void* root) { static_cast<RootType*>(root)->onUnload(); },                               \
        };                                                                                                             \
        return &desc;                                                                                                  \
    }
