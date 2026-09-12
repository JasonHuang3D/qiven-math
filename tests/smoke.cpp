#include <qiven/types.hpp>

#include <type_traits>

static_assert(std::is_same_v<qiven::f32, float>);
static_assert(std::is_same_v<qiven::f64, double>);

int main()
{
    return sizeof(qiven::usize) >= sizeof(unsigned int) ? 0 : 1;
}
