#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

namespace c_lo::detail {

template <typename AlwaysVoid, template <typename...> class T, typename ...Args>
struct is_detected_impl : std::false_type {};

template <template <typename...> typename T, typename ...Args>
struct is_detected_impl<std::void_t<T<Args...>>, T, Args...> : std::true_type {};

template <template <typename...> typename T, typename ...Args>
constexpr bool is_detected_v = is_detected_impl<void, T, Args...>::value;

}