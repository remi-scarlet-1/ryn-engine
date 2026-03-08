module;
#include "Prelude.h"
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

module Ryntime.Reflect;

RYN_SCOPE_FILE("TypeReflection");

namespace ryn {
    TypeRegistry& TypeRegistry::instance() {
        static TypeRegistry s_Instance;
        return s_Instance;
    }

    void TypeRegistry::registerType(TypeInfo typeInfo) {
        auto&            self = instance();
        std::unique_lock lock(self.m_Mutex);

        if (self.m_Types.contains(typeInfo.ID)) { return; }

        auto [it, inserted] = self.m_Types.emplace(typeInfo.ID, typeInfo);
        self.m_TypesByCategory[typeInfo.Category].push_back(&it->second);

        RYN_INFO("Registered type: {0}", typeInfo);
    }

    TypeInfo TypeRegistry::queryType(TypeID typeId) {
        auto&            self = instance();
        std::shared_lock lock(self.m_Mutex);

        if (auto it = self.m_Types.find(typeId); it != self.m_Types.end()) { return it->second; }

        RYN_ERROR("Failed querying TypeInfo for TypeID '{0}' as it does not exist in the registry.", typeId);
        return {};
    }

    Span<const TypeInfo* const> TypeRegistry::queryTypesByCategory(TypeCategory category) {
        auto&            self = instance();
        std::shared_lock lock(self.m_Mutex);

        if (auto it = self.m_TypesByCategory.find(category); it != self.m_TypesByCategory.end()) {
            const auto& vec = it->second;
            return Span(vec.data(), vec.size());
        }

        return {};
    }
}
