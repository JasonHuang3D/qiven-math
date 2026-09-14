#include <qiven/math/vec.hpp>

#include <cmath>
#include <concepts>
#include <cstdlib>
#include <limits>

using namespace qiven::math;

template <typename Left, typename Right>
concept dotable = requires(const Left& left, const Right& right) { dot(left, right); };

template <typename V>
concept crossable = requires(const V& left, const V& right) { cross(left, right); };

static_assert(dotable<Vec2f, Vec2f>);
static_assert(dotable<Vec3d, Vec3d>);
static_assert(!dotable<Vec2f, Vec2d>);
static_assert(crossable<Vec3f>);
static_assert(!crossable<Vec2f>);
static_assert(!crossable<Vec4f>);

constexpr bool constexpr_algorithms()
{
    const Vec2d a2 { 1.0, 2.0 };
    const Vec2d b2 { 3.0, 4.0 };
    const Vec3d x { 1.0, 0.0, 0.0 };
    const Vec3d y { 0.0, 1.0, 0.0 };
    return dot(a2, b2) == 11.0 && length_squared(Vec3d { 2.0, 3.0, 6.0 }) == 49.0 &&
           cross(x, y) == Vec3d { 0.0, 0.0, 1.0 } &&
           is_near(Vec2d { 1.0, 2.0 }, Vec2d { 1.25, 1.75 }, 0.25);
}

static_assert(constexpr_algorithms());

int main()
{
    const auto check = [](bool condition) {
        if (!condition)
            std::abort();
    };

    check(dot(Vec2d { 1.0, 2.0 }, Vec2d { 3.0, 4.0 }) == 11.0);
    check(dot(Vec4f { 1.0F, 2.0F, 3.0F, 4.0F }, Vec4f { 4.0F, 3.0F, 2.0F, 1.0F }) == 20.0F);
    check(cross(Vec3d { 1.0, 0.0, 0.0 }, Vec3d { 0.0, 1.0, 0.0 }) == Vec3d { 0.0, 0.0, 1.0 });

    check(length_squared(Vec3d { 2.0, 3.0, 6.0 }) == 49.0);
    check(length(Vec3d { 2.0, 3.0, 6.0 }) == 7.0);
    check(distance(Vec2d { 0.0, 0.0 }, Vec2d { 3.0, 4.0 }) == 5.0);

    const double huge        = std::numeric_limits<double>::max() / 2.0;
    const double huge_length = length(Vec2d { huge, huge });
    check(std::isfinite(huge_length));
    check(huge_length > huge);

    const double tiny        = std::numeric_limits<double>::min();
    const double tiny_length = length(Vec2d { tiny, tiny });
    check(tiny_length > tiny);

    const Vec3d normalized = normalize(Vec3d { 3.0, 4.0, 0.0 });
    check(is_near(normalized, Vec3d { 0.6, 0.8, 0.0 }, 1.0e-15));
    check(is_near(Vec3d { length(normalized), 0.0, 0.0 }, Vec3d { 1.0, 0.0, 0.0 }, 1.0e-15));

    const Vec2d huge_normalized = normalize(Vec2d { huge, huge });
    check(is_near(
        Vec2d { length(huge_normalized), 0.0 },
        Vec2d { 1.0, 0.0 },
        1.0e-15));

    Vec3d output { 9.0, 8.0, 7.0 };
    const Vec3d original_output = output;
    check(!try_normalize(Vec3d {}, output));
    check(output == original_output);

    const double infinity = std::numeric_limits<double>::infinity();
    check(!try_normalize(Vec3d { infinity, 1.0, 0.0 }, output));
    check(output == original_output);

    const double nan = std::numeric_limits<double>::quiet_NaN();
    check(!try_normalize(Vec3d { nan, 1.0, 0.0 }, output));
    check(output == original_output);

    check(try_normalize(Vec3d { 0.0, 5.0, 0.0 }, output));
    check(is_near(output, Vec3d { 0.0, 1.0, 0.0 }, 1.0e-15));

    check(std::isinf(length(Vec2d { infinity, nan })));
    check(std::isnan(length(Vec2d { nan, 1.0 })));

    check(is_near(Vec3d { 1.0, 2.0, 3.0 }, Vec3d { 1.125, 1.875, 3.25 }, 0.25));
    check(!is_near(Vec3d { 1.0, 2.0, 3.0 }, Vec3d { 1.125, 1.875, 3.25 }, 0.2));
    check(!is_near(Vec2d { infinity, 0.0 }, Vec2d { infinity, 0.0 }, 1.0));
    check(!is_near(Vec2d { nan, 0.0 }, Vec2d { nan, 0.0 }, 1.0));
    return 0;
}
