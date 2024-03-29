#pragma once

#include <c-lo/detail/range.hpp>
#include <c-lo/tied.hpp>

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace c_lo {

struct any_t
{};

template <typename T>
constexpr bool operator==(const any_t&, const T&) noexcept
{
    return true;
}
template <typename T>
constexpr bool operator==(const T&, const any_t&) noexcept
{
    return true;
}

constexpr any_t _;

struct arg_t : any_t
{};

constexpr arg_t arg;

template <typename... Args>
struct pattern
{
    using args_t = std::tuple<Args...>;
    constexpr pattern(Args... as)
        : args(std::forward<Args>(as)...)
    {}
    args_t args;
};

struct any_pattern
{
    using args_t = std::tuple<>;
};

namespace detail {

template <typename PatternTuple, typename Tuple, size_t... Ns>
bool equal_tuple_elements(const PatternTuple& t,
                          const Tuple& r,
                          std::index_sequence<Ns...>)
{
    return ((get(r, Ns) == std::get<Ns>(t)) && ...);
}

template <typename Tuple, typename... Args>
constexpr auto equal(const pattern<Args...>& p, const Tuple& t, try_t)
    -> decltype(p.args == tied(t))
{
    using args_t                = typename pattern<Args...>::args_t;
    constexpr auto pattern_size = std::tuple_size_v<args_t>;
    constexpr auto tuple_size =
        std::tuple_size_v<std::decay_t<decltype(tied(std::declval<Tuple>()))>>;

    static_assert(pattern_size == tuple_size,
                  "Pattern size should be same as tuple size");

    return p.args == tied(t);
}

template <typename Tuple, typename Range, size_t... Ns>
bool equal_elements(const Tuple& t, const Range& r, std::index_sequence<Ns...>)
{
    return ((get(r, Ns) == std::get<Ns>(t)) && ...);
}

template <typename Range, typename... Args>
constexpr auto equal(const pattern<Args...>& p, const Range& r, catch_t) -> bool
{
    if constexpr (sizeof...(Args) == 0) {
        return std::empty(r);
    } else {
        using args_t                = typename pattern<Args...>::args_t;
        constexpr auto pattern_size = std::tuple_size_v<args_t>;

        const auto r_size = detail::size(r);
        if (pattern_size != r_size)
            return false;

        return equal_elements(
            p.args, r, std::make_index_sequence<std::tuple_size_v<args_t>>{});
    }
}

} // namespace detail

template <typename Other, typename... Args>
bool operator==(const pattern<Args...>& lhs, const Other& rhs)
{
    return detail::equal(lhs, rhs, detail::try_t{});
}

template <typename Other, typename... Args>
bool operator==(const Other& lhs, const pattern<Args...>& rhs)
{
    return detail::equal(rhs, lhs, detail::try_t{});
}

template <typename Other, typename... Args>
bool operator!=(const pattern<Args...>& lhs, const Other& rhs)
{
    return !(lhs == rhs);
}

template <typename Other, typename... Args>
bool operator!=(const Other& lhs, const pattern<Args...>& rhs)
{
    return !(lhs == rhs);
}

template <typename Other>
constexpr bool operator==(const any_pattern&, const Other&)
{
    return true;
}

template <typename Other>
constexpr bool operator==(const Other&, const any_pattern&)
{
    return true;
}

template <typename Other>
constexpr bool operator!=(const any_pattern&, const Other&)
{
    return false;
}

template <typename Other>
constexpr bool operator!=(const Other&, const any_pattern&)
{
    return false;
}

} // namespace c_lo
