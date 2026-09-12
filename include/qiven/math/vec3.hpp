#pragma once

#include <qiven/contracts.hpp>
#include <qiven/math/scalar.hpp>
#include <qiven/types.hpp>

#include <concepts>

namespace qiven::math
{
template <floating_scalar T>
struct Vec3
{
    using value_type                        = T;
    static constexpr qiven::usize dimension = 3;
    T x {};
    T y {};
    T z {};

    [[nodiscard]] constexpr T& operator[](qiven::usize index) noexcept
    {
        QIVEN_ASSERT(index < dimension);
        if (index == 0)
            return x;
        if (index == 1)
            return y;
        return z;
    }
    [[nodiscard]] constexpr const T& operator[](qiven::usize index) const noexcept
    {
        QIVEN_ASSERT(index < dimension);
        if (index == 0)
            return x;
        if (index == 1)
            return y;
        return z;
    }
    [[nodiscard]] friend constexpr bool operator==(const Vec3&, const Vec3&) noexcept = default;
    [[nodiscard]] constexpr Vec3 operator+() const noexcept
    {
        return *this;
    }
    [[nodiscard]] constexpr Vec3 operator-() const noexcept
    {
        return { -x, -y, -z };
    }
    constexpr Vec3& operator+=(const Vec3& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    constexpr Vec3& operator-=(const Vec3& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template <typename S>
        requires std::same_as<S, T>
    constexpr Vec3& operator*=(S scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    template <typename S>
        requires std::same_as<S, T>
    constexpr Vec3& operator/=(S scalar) noexcept
    {
        QIVEN_ASSERT(scalar != T { 0 });
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
};

template <floating_scalar T>
[[nodiscard]] constexpr Vec3<T> operator+(Vec3<T> left, const Vec3<T>& right) noexcept
{
    return left += right;
}
template <floating_scalar T>
[[nodiscard]] constexpr Vec3<T> operator-(Vec3<T> left, const Vec3<T>& right) noexcept
{
    return left -= right;
}
template <floating_scalar T, typename S>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec3<T> operator*(Vec3<T> vector, S scalar) noexcept
{
    return vector *= scalar;
}
template <typename S, floating_scalar T>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec3<T> operator*(S scalar, Vec3<T> vector) noexcept
{
    return vector *= scalar;
}
template <floating_scalar T, typename S>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec3<T> operator/(Vec3<T> vector, S scalar) noexcept
{
    return vector /= scalar;
}

using Vec3f = Vec3<qiven::f32>;
using Vec3d = Vec3<qiven::f64>;
} // namespace qiven::math
