#pragma once

#include <c-lo/detail/meta.hpp>
#include <c-lo/detail/type_traits.hpp>

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace c_lo {

namespace detail {

template <typename T>
struct is_std_tuple : std::false_type
{};

template <typename... Ts>
struct is_std_tuple<std::tuple<Ts...>> : std::true_type
{};

template <typename T>
constexpr bool is_std_tuple_v = is_std_tuple<T>::value;

} // namespace detail

template <
    typename Tuple,
    typename = std::enable_if_t<detail::is_std_tuple_v<std::decay_t<Tuple>>>>
auto tied(Tuple&& t) -> decltype(auto)
{
    return std::forward<Tuple>(t);
}

} // namespace c_lo