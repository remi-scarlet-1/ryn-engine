module;
#include "Prelude.h"
#include <concepts>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

export module Ryntime.Core.Reflection:TypeReflection;
export import Ryntime.Core.Container;
export import :TypeCommon;

namespace ryn::reflection {
    class RYN_API ITypeReflection {
    public:
        virtual ~ITypeReflection()                                 = default;
        [[nodiscard]] virtual const char*  getTypeName() const     = 0;
        [[nodiscard]] virtual TypeID       getTypeID() const       = 0;
        [[nodiscard]] virtual TypeCategory getTypeCategory() const = 0;
    };
}

export namespace ryn::reflection {
    class RYN_API TypeRegistry {
    public:
        static TypeRegistry&                             instance();
        static void                                      registerType(TypeInfo typeInfo);
        [[nodiscard]] static TypeInfo                    queryType(TypeID typeId);
        [[nodiscard]] static Span<const TypeInfo* const> queryTypesByCategory(TypeCategory category);

    private:
        TypeRegistry()  = default;
        ~TypeRegistry() = default;

        mutable std::shared_mutex                                      m_Mutex;
        std::unordered_map<TypeID, TypeInfo>                           m_Types;
        std::unordered_map<TypeCategory, std::vector<const TypeInfo*>> m_TypesByCategory;
    };

    template<size_t N>
    struct TypeName {
        char value[N]{};
        consteval TypeName(const char (&str)[N]) {
            for (size_t i = 0; i < N; ++i)
                value[i] = str[i];
        }
        [[nodiscard]] constexpr const char* data() const { return value; }
    };

    template<typename T>
    concept IsStaticType = requires { typename T::StaticTypeAnchor; } && std::derived_from<T, typename T::StaticTypeAnchor>;

    template<typename T>
    concept IsDynamicBase = requires { typename T::DynamicBaseAnchor; } && std::derived_from<T, typename T::DynamicBaseAnchor>;

    template<typename T>
    concept IsDynamicType = requires { typename T::DynamicTypeAnchor; } && std::derived_from<T, typename T::DynamicTypeAnchor>;

    template<typename T>
    concept IsType = IsStaticType<T> || IsDynamicType<T>;

    template<TypeCategory Category, TypeName Name, bool IsDynamic = false>
    struct StaticType {
        using StaticTypeAnchor = StaticType<Category, Name>;

        static constexpr const char*  TYPE_NAME     = Name.data();
        static constexpr TypeID       TYPE_ID       = TypeID::generateHash(Name.data());
        static constexpr TypeCategory TYPE_CATEGORY = Category;
        static constexpr bool         IS_DYNAMIC    = IsDynamic;

        static bool getTypeStatus() {
            TypeRegistry::registerType({.Name = TYPE_NAME, .ID = TYPE_ID, .Category = TYPE_CATEGORY, .IsDynamic = IS_DYNAMIC});
            return true;
        }
    };

    template<TypeCategory Category>
    class DynamicBase : public ITypeReflection {
    public:
        using DynamicBaseAnchor = DynamicBase<Category>;

        static constexpr TypeCategory TYPE_CATEGORY = Category;

        [[nodiscard]] TypeCategory getTypeCategory() const final { return Category; }
    };

    template<IsDynamicBase Base, TypeName Name>
    class DynamicType : public Base, public StaticType<Base::TYPE_CATEGORY, Name, true> {
    public:
        using DynamicTypeAnchor = DynamicType<Base, Name>;
        using Meta              = StaticType<Base::TYPE_CATEGORY, Name, true>;

        using Meta::getTypeStatus;
        using Meta::IS_DYNAMIC;
        using Meta::TYPE_CATEGORY;
        using Meta::TYPE_ID;
        using Meta::TYPE_NAME;

        [[nodiscard]] const char* getTypeName() const final { return TYPE_NAME; }
        [[nodiscard]] TypeID      getTypeID() const final { return TYPE_ID; }

    protected:
        constexpr DynamicType() noexcept { (void)s_Registered; }

    private:
        static inline bool s_Registered = getTypeStatus();
    };
}