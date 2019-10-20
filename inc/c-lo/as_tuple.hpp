#pragma once

#include <c-lo/detail/meta.hpp>
#include <c-lo/detail/type_traits.hpp>

#include <array>
#include <cstddef>
#include <tuple>
#include <utility>
#include <type_traits>

namespace c_lo {

template <typename T>
struct is_std_tuple : std::false_type {};

template <typename ...Ts>
struct is_std_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T>
constexpr bool is_std_tuple_v = is_std_tuple<T>::value;

template <typename Tuple, typename = std::enable_if_t<is_std_tuple_v<std::decay_t<Tuple>>>>
auto forward_tuple(Tuple&& t) -> decltype(auto)
{
    return std::forward<Tuple>(t);
}

template <typename Tuple>
auto as_tuple(Tuple&& tuple) CLO_RETURN(forward_tuple(std::forward<Tuple>(tuple)))

template <typename Array, size_t ...Ns>
auto array_as_tuple(Array&& arr, std::index_sequence<Ns...>)
{
    return std::make_tuple( std::get<Ns>(arr)... );
}

template <typename Array, typename = std::enable_if_t<detail::is_array_v<std::decay_t<Array>>>>
auto array_as_tuple(Array&& arr)
{
    return array_as_tuple( std::forward<Array>(arr), std::make_index_sequence<std::tuple_size_v<std::decay_t<Array>>>{} );
}

template <typename Array>
auto as_tuple(Array&& arr) CLO_RETURN(array_as_tuple(std::forward<Array>(arr)))

}