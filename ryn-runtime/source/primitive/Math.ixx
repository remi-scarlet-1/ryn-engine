module;
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

export module Ryntime.Primitive:Math;

namespace ryn {
    template<typename GLMType, typename T>
    [[nodiscard]] constexpr GLMType& cast(T& type) noexcept {
        return static_cast<GLMType&>(type);
    }

    template<typename GLMType, typename T>
    [[nodiscard]] constexpr const GLMType& cast(const T& type) noexcept {
        return static_cast<const GLMType&>(type);
    }

    template<typename Derived, typename GLMType, size_t Footprint>
    struct alignas(Footprint) VectorBase : GLMType {
        using T = GLMType::value_type;

        // Constructors
        using GLMType::GLMType;
        constexpr VectorBase() noexcept = default;
        constexpr VectorBase(const GLMType vector) noexcept : GLMType(vector) {}
        explicit constexpr VectorBase(const float scalar) noexcept : GLMType(scalar) {}

        // Assignment Operators
        constexpr Derived& operator+=(const Derived& o) noexcept { return cast<GLMType>(*this) += cast<GLMType>(o), cast<Derived>(*this); }
        constexpr Derived& operator-=(const Derived& o) noexcept { return cast<GLMType>(*this) -= cast<GLMType>(o), cast<Derived>(*this); }
        constexpr Derived& operator*=(float s) noexcept { return cast<GLMType>(*this) *= s, cast<Derived>(*this); }
        constexpr Derived& operator/=(float s) noexcept { return cast<GLMType>(*this) /= s, cast<Derived>(*this); }

        // Binary Operators
        [[nodiscard]] friend constexpr Derived operator+(Derived a, const Derived& b) noexcept { return a += b; }
        [[nodiscard]] friend constexpr Derived operator-(Derived a, const Derived& b) noexcept { return a -= b; }
        [[nodiscard]] friend constexpr Derived operator*(Derived v, float scalar) noexcept { return v *= scalar; }
        [[nodiscard]] friend constexpr Derived operator*(float scalar, Derived v) noexcept { return v *= scalar; }
        [[nodiscard]] friend constexpr Derived operator/(Derived v, float scalar) noexcept { return v /= scalar; }

        // Member Utilities
        [[nodiscard]] constexpr Derived normalized() const noexcept { return Derived(glm::normalize(cast<GLMType>(*this))); }
        [[nodiscard]] constexpr float   length() const noexcept { return glm::length(cast<GLMType>(*this)); }
        [[nodiscard]] constexpr float   lengthSqr() const noexcept { return glm::dot(cast<GLMType>(*this), cast<GLMType>(*this)); }

        // Static Accessors
        [[nodiscard]] static constexpr Derived one() { return Derived{1.0f}; }
        [[nodiscard]] static constexpr Derived zero() { return Derived{0.0f}; }

        // Static Utilities
        [[nodiscard]] static constexpr float dot(const Derived& a, const Derived& b) noexcept {
            return glm::dot(cast<GLMType>(a), cast<GLMType>(b));
        }

        [[nodiscard]] static constexpr float distance(const Derived& a, const Derived& b) noexcept {
            return glm::distance(cast<GLMType>(a), cast<GLMType>(b));
        }

        [[nodiscard]] static constexpr Derived lerp(const Derived& a, const Derived& b, float t) noexcept {
            return Derived(glm::mix(cast<GLMType>(a), cast<GLMType>(b), t));
        }
    };
}

export namespace ryn {
    struct alignas(8) Vector2 : VectorBase<Vector2, glm::vec2, 8> {
        using VectorBase::VectorBase;
        constexpr Vector2() noexcept = default;

        // Static Accessors
        [[nodiscard]] static constexpr Vector2 right() { return Vector2{1.0f, 0.0f}; }
        [[nodiscard]] static constexpr Vector2 left() { return Vector2{-1.0f, 0.0f}; }
        [[nodiscard]] static constexpr Vector2 up() { return Vector2{0.0f, 1.0f}; }
        [[nodiscard]] static constexpr Vector2 down() { return Vector2{0.0f, -1.0f}; }
    };

    static_assert(std::is_standard_layout_v<Vector2>);
    static_assert(sizeof(Vector2) == 8);

    struct alignas(8) Vector2Int : VectorBase<Vector2Int, glm::ivec2, 8> {
        using VectorBase::VectorBase;
        constexpr Vector2Int() noexcept = default;

        // Static Accessors
        [[nodiscard]] static constexpr Vector2Int right() { return Vector2Int{1, 0}; }
        [[nodiscard]] static constexpr Vector2Int left() { return Vector2Int{-1, 0}; }
        [[nodiscard]] static constexpr Vector2Int up() { return Vector2Int{0, 1}; }
        [[nodiscard]] static constexpr Vector2Int down() { return Vector2Int{0, -1}; }
    };

    static_assert(std::is_standard_layout_v<Vector2Int>);
    static_assert(sizeof(Vector2Int) == 8);

    struct alignas(16) Vector3 : VectorBase<Vector3, glm::vec3, 16> {
        using VectorBase::VectorBase;
        constexpr Vector3() noexcept = default;

        // Static Accessors
        [[nodiscard]] static constexpr Vector3 right() { return Vector3{1.0f, 0.0f, 0.0f}; }
        [[nodiscard]] static constexpr Vector3 left() { return Vector3{-1.0f, 0.0f, 0.0f}; }
        [[nodiscard]] static constexpr Vector3 up() { return Vector3{0.0f, 1.0f, 0.0f}; }
        [[nodiscard]] static constexpr Vector3 down() { return Vector3{0.0f, -1.0f, 0.0f}; }
        [[nodiscard]] static constexpr Vector3 forward() { return Vector3{0.0f, 0.0f, 1.0f}; }
        [[nodiscard]] static constexpr Vector3 back() { return Vector3{0.0f, 0.0f, -1.0f}; }
    };

    static_assert(std::is_standard_layout_v<Vector3>);
    static_assert(sizeof(Vector3) == 16);

    struct alignas(16) Vector3Int : VectorBase<Vector3Int, glm::ivec3, 16> {
        using VectorBase::VectorBase;
        constexpr Vector3Int() noexcept = default;

        // Static Accessors
        [[nodiscard]] static constexpr Vector3Int right() { return Vector3Int{1, 0, 0}; }
        [[nodiscard]] static constexpr Vector3Int left() { return Vector3Int{-1, 0, 0}; }
        [[nodiscard]] static constexpr Vector3Int up() { return Vector3Int{0, 1, 0}; }
        [[nodiscard]] static constexpr Vector3Int down() { return Vector3Int{0, -1, 0}; }
        [[nodiscard]] static constexpr Vector3Int forward() { return Vector3Int{0, 0, 1}; }
        [[nodiscard]] static constexpr Vector3Int back() { return Vector3Int{0, 0, -1}; }
    };

    static_assert(std::is_standard_layout_v<Vector3Int>);
    static_assert(sizeof(Vector3Int) == 16);

    struct alignas(16) Vector4 : VectorBase<Vector4, glm::vec4, 16> {
        using VectorBase::VectorBase;
        constexpr Vector4() noexcept = default;
    };

    static_assert(std::is_standard_layout_v<Vector4>);
    static_assert(sizeof(Vector4) == 16);

    struct alignas(16) Vector4Int : VectorBase<Vector4Int, glm::ivec4, 16> {
        using VectorBase::VectorBase;
        constexpr Vector4Int() noexcept = default;
    };

    static_assert(std::is_standard_layout_v<Vector4Int>);
    static_assert(sizeof(Vector4Int) == 16);

    struct alignas(16) Quaternion : glm::quat {
        // Constructors
        using glm::quat::quat;
        constexpr Quaternion() noexcept = default;
        constexpr Quaternion(const glm::quat& q) noexcept : glm::quat(q) {}

        // Binary Operators
        [[nodiscard]] friend constexpr Quaternion operator*(const Quaternion& a, const Quaternion& b) noexcept {
            return cast<glm::quat>(a) * cast<glm::quat>(b);
        }

        [[nodiscard]] friend constexpr Vector3 operator*(const Quaternion& q, const Vector3& v) noexcept {
            return cast<glm::quat>(q) * cast<glm::vec3>(v);
        }

        // Member Utilities
        [[nodiscard]] constexpr Quaternion normalized() const noexcept { return glm::normalize(cast<glm::quat>(*this)); }
        [[nodiscard]] constexpr Quaternion inversed() const noexcept { return glm::inverse(cast<glm::quat>(*this)); }
        [[nodiscard]] constexpr Vector3    eulerAngles() const noexcept {
            const glm::vec3 rad = glm::eulerAngles(cast<glm::quat>(*this));
            return {glm::degrees(rad.x), glm::degrees(rad.y), glm::degrees(rad.z)};
        }

        // Static Accessors
        [[nodiscard]] static constexpr Quaternion identity() { return {1.0f, 0.0f, 0.0f, 0.0f}; }

        [[nodiscard]] static constexpr Quaternion fromAngleAxis(const float angleDegrees, const Vector3& axis) noexcept {
            return glm::angleAxis(glm::radians(angleDegrees), cast<glm::vec3>(axis));
        }

        [[nodiscard]] static constexpr Quaternion fromEulerAngles(const float x, const float y, const float z) noexcept {
            return {glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z))};
        }

        [[nodiscard]] static constexpr Quaternion fromLookRotation(const Vector3& forward, const Vector3& up = Vector3::up()) noexcept {
            return glm::quatLookAt(glm::normalize(cast<glm::vec3>(forward)), cast<glm::vec3>(up));
        }

        // Static Utilities
        [[nodiscard]] static constexpr float angle(const Quaternion& a, const Quaternion& b) noexcept {
            return glm::degrees(glm::angle(glm::inverse(cast<glm::quat>(a)) * cast<glm::quat>(b)));
        }

        [[nodiscard]] static constexpr float dot(const Quaternion& a, const Quaternion& b) noexcept {
            return glm::dot(cast<glm::quat>(a), cast<glm::quat>(b));
        }
    };

    static_assert(std::is_standard_layout_v<Quaternion>);
    static_assert(sizeof(Quaternion) == 16);

    struct alignas(64) Matrix4x4 : glm::mat4 {
        // Constructors
        using glm::mat4::mat4;
        constexpr Matrix4x4() noexcept = default;
        constexpr Matrix4x4(const glm::mat4& m) noexcept : glm::mat4(m) {}

        // Assignment Operators
        constexpr Matrix4x4& operator*=(const Quaternion& q) noexcept {
            return *this = cast<glm::mat4>(*this) * glm::mat4_cast(cast<glm::quat>(q));
        }

        // Binary Operators
        [[nodiscard]] friend constexpr Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b) noexcept {
            return cast<glm::mat4>(a) * cast<glm::mat4>(b);
        }
        [[nodiscard]] friend constexpr Matrix4x4 operator*(Matrix4x4 m, const Quaternion& q) noexcept { return m *= q; }
        [[nodiscard]] friend constexpr Vector4   operator*(const Matrix4x4& m, const Vector4& v) noexcept {
            return cast<glm::mat4>(m) * cast<glm::vec4>(v);
        }

        // Member Utilities
        [[nodiscard]] constexpr Matrix4x4 inversed() const noexcept { return glm::inverse(cast<glm::mat4>(*this)); }
        [[nodiscard]] constexpr Matrix4x4 transposed() const noexcept { return glm::transpose(cast<glm::mat4>(*this)); }

        // Static Accessors
        [[nodiscard]] static constexpr Matrix4x4 identity() { return glm::identity<glm::mat4>(); }

        [[nodiscard]] static constexpr Matrix4x4 fromLookAt(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& up) noexcept {
            return glm::lookAt(cast<glm::vec3>(cameraPos), cast<glm::vec3>(targetPos), cast<glm::vec3>(up));
        }

        [[nodiscard]] static constexpr Matrix4x4 fromTRS(const Vector3& translation, const Quaternion& rotation, const Vector3& scale) noexcept {
            glm::mat4 matrix = glm::translate(glm::mat4(1.0f), cast<glm::vec3>(translation));
            matrix           = matrix * glm::mat4_cast(cast<glm::quat>(rotation));
            matrix           = glm::scale(matrix, cast<glm::vec3>(scale));
            return matrix;
        }

        [[nodiscard]] static constexpr Matrix4x4 fromPerspective(const float fovyDegrees,
                                                                 const float aspect,
                                                                 const float nearClip,
                                                                 const float farClip) noexcept {
            return glm::perspective(glm::radians(fovyDegrees), aspect, nearClip, farClip);
        }

        [[nodiscard]] static constexpr Matrix4x4 fromOrthographic(const float left,
                                                                  const float right,
                                                                  const float bottom,
                                                                  const float top,
                                                                  const float zNear,
                                                                  const float zFar) noexcept {
            return glm::ortho(left, right, bottom, top, zNear, zFar);
        }
    };

    static_assert(sizeof(Matrix4x4) == 64);
    static_assert(std::is_standard_layout_v<Matrix4x4>);
}
