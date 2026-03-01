#pragma once

#define RYN_DEFINE_CONCAT_IMPL(a, b) a##b
#define RYN_DEFINE_CONCAT(a, b)      RYN_DEFINE_CONCAT_IMPL(a, b)

#define RYN_TYPE_REFLECTION_KEEP(Type)                                                                                                     \
    static_assert(::ryn::reflection::IsType<Type>,                                                                                         \
                  "REFLECTION ERROR: " #Type " is not reflectable. "                                                                       \
                  "Ensure it inherits either from ryn::reflection::StaticType<Category, Name> or "                                         \
                  "ryn::reflection::DynamicType<Base, Name>");                                                                             \
    namespace {                                                                                                                            \
        static const bool RYN_DEFINE_CONCAT(ryn_type_reflection_keep_, __LINE__) = Type::getTypeStatus();                                  \
    }