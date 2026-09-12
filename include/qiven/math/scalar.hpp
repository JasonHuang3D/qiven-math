#pragma once

#include <concepts>

namespace qiven::math
{
template <typename T>
concept floating_scalar = std::same_as<T, float> || std::same_as<T, double>;
} // namespace qiven::math
