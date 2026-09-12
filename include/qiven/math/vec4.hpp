#pragma once

#include <qiven/contracts.hpp>
#include <qiven/math/scalar.hpp>
#include <qiven/types.hpp>

#include <concepts>

namespace qiven::math
{
template <floating_scalar T>
struct Vec4
{
    using value_type                        = T;
    static constexpr qiven::usize dimension = 4;
    T x {};
    T y {};
    T z {};
    T w {};

    [[nodiscard]] constexpr T& operator[](qiven::usize index) noexcept
    {
        QIVEN_ASSERT(index < dimension);
        if (index == 0)
            return x;
        if (index == 1)
            return y;
        if (index == 2)
            return z;
        return w;
    }
    [[nodiscard]] constexpr const T& operator[](qiven::usize index) const noexcept
    {
        QIVEN_ASSERT(index < dimension);
        if (index == 0)
            return x;
        if (index == 1)
            return y;
        if (index == 2)
            return z;
        return w;
    }
    [[nodiscard]] friend constexpr bool operator==(const Vec4&, const Vec4&) noexcept = default;
    [[nodiscard]] constexpr Vec4 operator+() const noexcept
    {
        return *this;
    }
    [[nodiscard]] constexpr Vec4 operator-() const noexcept
    {
        return { -x, -y, -z, -w };
    }
    constexpr Vec4& operator+=(const Vec4& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    constexpr Vec4& operator-=(const Vec4& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    template <typename S>
        requires std::same_as<S, T>
    constexpr Vec4& operator*=(S scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    template <typename S>
        requires std::same_as<S, T>
    constexpr Vec4& operator/=(S scalar) noexcept
    {
        QIVEN_ASSERT(scalar != T { 0 });
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
};

template <floating_scalar T>
[[nodiscard]] constexpr Vec4<T> operator+(Vec4<T> left, const Vec4<T>& right) noexcept
{
    return left += right;
}
template <floating_scalar T>
[[nodiscard]] constexpr Vec4<T> operator-(Vec4<T> left, const Vec4<T>& right) noexcept
{
    return left -= right;
}
template <floating_scalar T, typename S>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec4<T> operator*(Vec4<T> vector, S scalar) noexcept
{
    return vector *= scalar;
}
template <typename S, floating_scalar T>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec4<T> operator*(S scalar, Vec4<T> vector) noexcept
{
    return vector *= scalar;
}
template <floating_scalar T, typename S>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec4<T> operator/(Vec4<T> vector, S scalar) noexcept
{
    return vector /= scalar;
}

using Vec4f = Vec4<qiven::f32>;
using Vec4d = Vec4<qiven::f64>;
} // namespace qiven::math
