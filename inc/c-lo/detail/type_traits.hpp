#pragma once

#include <array>
#include <type_traits>

namespace c_lo::detail {

template <typename T>
struct is_array : std::is_array<T> {};

template <typename T, std::size_t Index>
struct is_array<std::array<T, Index>> : std::true_type {};

template <typename T>
constexpr bool is_array_v = is_array<T>::value;

}