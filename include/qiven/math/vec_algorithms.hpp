#pragma once

#include <qiven/contracts.hpp>
#include <qiven/math/vec2.hpp>
#include <qiven/math/vec3.hpp>
#include <qiven/math/vec4.hpp>
#include <qiven/types.hpp>

#include <cmath>
#include <limits>

namespace qiven::math
{
namespace detail
{
template <typename V>
inline constexpr bool is_qiven_vector_v = false;

template <floating_scalar T>
inline constexpr bool is_qiven_vector_v<Vec2<T>> = true;

template <floating_scalar T>
inline constexpr bool is_qiven_vector_v<Vec3<T>> = true;

template <floating_scalar T>
inline constexpr bool is_qiven_vector_v<Vec4<T>> = true;

template <typename V>
concept qiven_vector = is_qiven_vector_v<V>;

template <qiven_vector V>
[[nodiscard]] constexpr typename V::value_type dot_impl(const V& left, const V& right) noexcept
{
    using T = typename V::value_type;
    T result { 0 };
    for (qiven::usize index = 0; index < V::dimension; ++index)
        result += left[index] * right[index];
    return result;
}

template <typename T>
[[nodiscard]] constexpr T absolute_value(T value) noexcept
{
    return value < T { 0 } ? -value : value;
}

template <qiven_vector V>
[[nodiscard]] bool all_finite(const V& vector) noexcept
{
    for (qiven::usize index = 0; index < V::dimension; ++index)
    {
        if (!std::isfinite(vector[index]))
            return false;
    }
    return true;
}

template <qiven_vector V>
[[nodiscard]] constexpr typename V::value_type max_abs_component(const V& vector) noexcept
{
    using T = typename V::value_type;
    T scale { 0 };
    for (qiven::usize index = 0; index < V::dimension; ++index)
    {
        const T magnitude = absolute_value(vector[index]);
        if (magnitude > scale)
            scale = magnitude;
    }
    return scale;
}

template <qiven_vector V>
[[nodiscard]] V normalized_from_scale(const V& vector, typename V::value_type scale) noexcept
{
    using T = typename V::value_type;
    V scaled {};
    for (qiven::usize index = 0; index < V::dimension; ++index)
        scaled[index] = vector[index] / scale;

    const T scaled_length = std::sqrt(dot_impl(scaled, scaled));
    V result {};
    for (qiven::usize index = 0; index < V::dimension; ++index)
        result[index] = scaled[index] / scaled_length;
    return result;
}
} // namespace detail

template <detail::qiven_vector V>
[[nodiscard]] constexpr typename V::value_type dot(const V& left, const V& right) noexcept
{
    return detail::dot_impl(left, right);
}

template <floating_scalar T>
[[nodiscard]] constexpr Vec3<T> cross(const Vec3<T>& left, const Vec3<T>& right) noexcept
{
    return {
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x,
    };
}

template <detail::qiven_vector V>
[[nodiscard]] constexpr typename V::value_type length_squared(const V& vector) noexcept
{
    return detail::dot_impl(vector, vector);
}

template <detail::qiven_vector V>
[[nodiscard]] typename V::value_type length(const V& vector) noexcept
{
    using T = typename V::value_type;

    bool has_infinity = false;
    bool has_nan      = false;
    T scale { 0 };
    for (qiven::usize index = 0; index < V::dimension; ++index)
    {
        const T component = vector[index];
        if (std::isinf(component))
        {
            has_infinity = true;
            continue;
        }
        if (std::isnan(component))
        {
            has_nan = true;
            continue;
        }

        const T magnitude = detail::absolute_value(component);
        if (magnitude > scale)
            scale = magnitude;
    }

    if (has_infinity)
        return std::numeric_limits<T>::infinity();
    if (has_nan)
        return std::numeric_limits<T>::quiet_NaN();
    if (scale == T { 0 })
        return T { 0 };

    T scaled_square_sum { 0 };
    for (qiven::usize index = 0; index < V::dimension; ++index)
    {
        const T scaled = vector[index] / scale;
        scaled_square_sum += scaled * scaled;
    }
    return scale * std::sqrt(scaled_square_sum);
}

template <detail::qiven_vector V>
[[nodiscard]] V normalize(const V& vector) noexcept
{
    QIVEN_ASSERT(detail::all_finite(vector));
    const typename V::value_type scale = detail::max_abs_component(vector);
    QIVEN_ASSERT(scale != typename V::value_type { 0 });
    return detail::normalized_from_scale(vector, scale);
}

template <detail::qiven_vector V>
[[nodiscard]] bool try_normalize(const V& vector, V& output) noexcept
{
    if (!detail::all_finite(vector))
        return false;

    const typename V::value_type scale = detail::max_abs_component(vector);
    if (scale == typename V::value_type { 0 })
        return false;

    const V normalized = detail::normalized_from_scale(vector, scale);
    output             = normalized;
    return true;
}

template <detail::qiven_vector V>
[[nodiscard]] typename V::value_type distance(const V& left, const V& right) noexcept
{
    return length(right - left);
}

template <detail::qiven_vector V>
[[nodiscard]] constexpr bool is_near(
    const V& left,
    const V& right,
    typename V::value_type tolerance) noexcept
{
    using T = typename V::value_type;
    QIVEN_ASSERT(tolerance >= T { 0 } && tolerance <= std::numeric_limits<T>::max());

    for (qiven::usize index = 0; index < V::dimension; ++index)
    {
        const T difference = left[index] - right[index];
        if (!(difference <= tolerance && difference >= -tolerance))
            return false;
    }
    return true;
}
} // namespace qiven::math
