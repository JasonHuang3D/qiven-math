#pragma once

#include <qiven/contracts.hpp>
#include <qiven/math/scalar.hpp>
#include <qiven/types.hpp>

#include <concepts>

namespace qiven::math
{
template <floating_scalar T>
struct Vec2
{
    using value_type                        = T;
    static constexpr qiven::usize dimension = 2;
    T x {};
    T y {};

    [[nodiscard]] constexpr T& operator[](qiven::usize index) noexcept
    {
        QIVEN_ASSERT(index < dimension);
        return index == 0 ? x : y;
    }
    [[nodiscard]] constexpr const T& operator[](qiven::usize index) const noexcept
    {
        QIVEN_ASSERT(index < dimension);
        return index == 0 ? x : y;
    }
    [[nodiscard]] friend constexpr bool operator==(const Vec2&, const Vec2&) noexcept = default;
    [[nodiscard]] constexpr Vec2 operator+() const noexcept
    {
        return *this;
    }
    [[nodiscard]] constexpr Vec2 operator-() const noexcept
    {
        return { -x, -y };
    }
    constexpr Vec2& operator+=(const Vec2& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    constexpr Vec2& operator-=(const Vec2& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template <typename S>
        requires std::same_as<S, T>
    constexpr Vec2& operator*=(S scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    template <typename S>
        requires std::same_as<S, T>
    constexpr Vec2& operator/=(S scalar) noexcept
    {
        QIVEN_ASSERT(scalar != T { 0 });
        x /= scalar;
        y /= scalar;
        return *this;
    }
};

template <floating_scalar T>
[[nodiscard]] constexpr Vec2<T> operator+(Vec2<T> left, const Vec2<T>& right) noexcept
{
    return left += right;
}
template <floating_scalar T>
[[nodiscard]] constexpr Vec2<T> operator-(Vec2<T> left, const Vec2<T>& right) noexcept
{
    return left -= right;
}
template <floating_scalar T, typename S>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec2<T> operator*(Vec2<T> vector, S scalar) noexcept
{
    return vector *= scalar;
}
template <typename S, floating_scalar T>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec2<T> operator*(S scalar, Vec2<T> vector) noexcept
{
    return vector *= scalar;
}
template <floating_scalar T, typename S>
    requires std::same_as<S, T>
[[nodiscard]] constexpr Vec2<T> operator/(Vec2<T> vector, S scalar) noexcept
{
    return vector /= scalar;
}

using Vec2f = Vec2<qiven::f32>;
using Vec2d = Vec2<qiven::f64>;
} // namespace qiven::math
