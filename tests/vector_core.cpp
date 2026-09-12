#include <qiven/math/vec.hpp>

#include <concepts>
#include <cstdlib>
#include <type_traits>

using namespace qiven::math;

static_assert(floating_scalar<float>);
static_assert(floating_scalar<double>);
static_assert(!floating_scalar<const float>);
static_assert(!floating_scalar<volatile float>);
static_assert(!floating_scalar<long double>);
static_assert(!floating_scalar<int>);
static_assert(!floating_scalar<unsigned>);
static_assert(!floating_scalar<bool>);

template <typename V, typename T, qiven::usize N>
constexpr bool has_required_properties()
{
    return std::is_aggregate_v<V> && std::is_standard_layout_v<V> && std::is_trivially_copyable_v<V> &&
           std::same_as<typename V::value_type, T> && V::dimension == N && sizeof(V) == N * sizeof(T) &&
           alignof(V) == alignof(T);
}

static_assert(has_required_properties<Vec2f, float, 2>());
static_assert(has_required_properties<Vec2d, double, 2>());
static_assert(has_required_properties<Vec3f, float, 3>());
static_assert(has_required_properties<Vec3d, double, 3>());
static_assert(has_required_properties<Vec4f, float, 4>());
static_assert(has_required_properties<Vec4d, double, 4>());

static_assert(std::same_as<Vec2f, Vec2<qiven::f32>>);
static_assert(std::same_as<Vec2d, Vec2<qiven::f64>>);
static_assert(std::same_as<Vec3f, Vec3<qiven::f32>>);
static_assert(std::same_as<Vec3d, Vec3<qiven::f64>>);
static_assert(std::same_as<Vec4f, Vec4<qiven::f32>>);
static_assert(std::same_as<Vec4d, Vec4<qiven::f64>>);

static_assert(!std::convertible_to<Vec2f, Vec2d> && !std::convertible_to<Vec2d, Vec2f>);
static_assert(!std::convertible_to<Vec3f, Vec3d> && !std::convertible_to<Vec3d, Vec3f>);
static_assert(!std::convertible_to<Vec4f, Vec4d> && !std::convertible_to<Vec4d, Vec4f>);

template <typename V, typename S>
concept multipliable = requires(V value, S scalar) { value* scalar; };

static_assert(multipliable<Vec2f, float>);
static_assert(!multipliable<Vec2f, double>);
static_assert(!multipliable<Vec2d, float>);

constexpr bool constexpr_operations()
{
    Vec3d value { 1.0, 2.0, 3.0 };
    value[1]               = 4.0;
    const Vec3d sum        = value + Vec3d { 2.0, 3.0, 4.0 };
    const Vec3d difference = sum - Vec3d { 1.0, 1.0, 1.0 };
    const Vec3d scaled     = 2.0 * difference;
    const Vec3d divided    = scaled / 2.0;
    return +divided == Vec3d { 2.0, 6.0, 6.0 } && -divided == Vec3d { -2.0, -6.0, -6.0 };
}

static_assert(constexpr_operations());

int main()
{
    const auto check = [](bool condition) {
        if (!condition)
            std::abort();
    };

    const Vec2d zero2 {};
    const Vec3d zero3 {};
    const Vec4d zero4 {};
    check(zero2 == Vec2d { 0.0, 0.0 });
    check(zero3 == Vec3d { 0.0, 0.0, 0.0 });
    check(zero4 == Vec4d { 0.0, 0.0, 0.0, 0.0 });

    Vec4f value { 1.0F, 2.0F, 3.0F, 4.0F };
    check(&value[0] == &value.x);
    check(&value[1] == &value.y);
    check(&value[2] == &value.z);
    check(&value[3] == &value.w);
    value[2]              = 8.0F;
    const Vec4f& constant = value;
    check(&constant[2] == &constant.z);
    check(value != Vec4f { 1.0F, 2.0F, 3.0F, 4.0F });

    value += Vec4f { 1.0F, 1.0F, 1.0F, 1.0F };
    value -= Vec4f { 1.0F, 2.0F, 3.0F, 4.0F };
    value *= 2.0F;
    value /= 2.0F;
    check(value == Vec4f { 1.0F, 1.0F, 6.0F, 1.0F });
    check(value * 2.0F == 2.0F * value);
    check(value / 2.0F == Vec4f { 0.5F, 0.5F, 3.0F, 0.5F });
    return 0;
}
