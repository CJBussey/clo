#pragma once

#include <c-lo/detail/macros.hpp>
#include <c-lo/detail/meta.hpp>
#include <c-lo/pattern.hpp>
#include <c-lo/tied.hpp>

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>

namespace c_lo {

namespace detail {

template <typename T>
struct is_arg : std::false_type
{};

template <>
struct is_arg<arg_t> : std::true_type
{};

template <typename T>
constexpr bool is_arg_v = is_arg<T>::value;

template <typename Args, std::size_t Index>
struct make_arg_indexes_impl;

template <typename Args>
struct make_arg_indexes_impl<Args, 0>
{
    using type =
        append_to_index_sequence_if_t<0,
                                      std::index_sequence<>,
                                      is_arg_v<std::tuple_element_t<0, Args>>>;
};

template <typename Args, std::size_t Index>
struct make_arg_indexes_impl
{
    using type = append_to_index_sequence_if_t<
        Index,
        typename make_arg_indexes_impl<Args, Index - 1>::type,
        is_arg_v<std::tuple_element_t<Index, Args>>>;
};

template <typename Args, typename AlwaysVoid>
struct make_arg_indexes
{
    using type =
        typename make_arg_indexes_impl<Args, std::tuple_size_v<Args> - 1>::type;
};

template <typename Args>
struct make_arg_indexes<Args, std::enable_if_t<std::tuple_size_v<Args> == 0>>
{
    using type = std::index_sequence<>;
};

template <typename Args>
using arg_indexes_t = typename make_arg_indexes<Args, void>::type;

template <typename T>
constexpr bool all_any_elements_v = false;

template <template <typename...> typename Pattern, typename... Args>
constexpr bool all_any_elements_v<Pattern<Args...>> =
    (std::is_base_of_v<any_t, Args> && ...);

template <typename... Matchers>
constexpr bool has_pattern_with_all_any_elements_v =
    (all_any_elements_v<std::decay_t<typename Matchers::pattern_type>> || ...);

template <typename Pattern>
constexpr bool is_any_pattern_v = false;

template <>
constexpr bool is_any_pattern_v<any_pattern> = true;

template <typename... Matchers>
constexpr bool has_any_pattern_v =
    (is_any_pattern_v<std::decay_t<typename Matchers::pattern_type>> || ...);

template <typename Func, typename TupleValue, std::size_t... Ns>
constexpr decltype(auto)
apply_tuple_args(Func&& func, TupleValue&& value, std::index_sequence<Ns...>)
{
    return std::invoke(std::forward<Func>(func),
                       std::get<Ns>(std::forward<TupleValue>(value))...);
}

template <typename Func, typename Range, std::size_t... Ns>
constexpr decltype(auto)
apply_range_args(Func&& func, Range&& r, std::index_sequence<Ns...>)
{
    return std::invoke(std::forward<Func>(func), get(r, Ns)...);
}

template <typename T>
using free_tied = decltype(tied(std::declval<T>()));

template <typename Matcher, typename Value>
constexpr decltype(auto) apply_value(Matcher&& matcher, Value&& v)
{
    if constexpr (is_detected_v<free_tied, Value>) {
        return apply_tuple_args(
            std::forward<Matcher>(matcher).handler,
            tied(std::forward<Value>(v)),
            arg_indexes_t<
                typename std::decay_t<Matcher>::pattern_type::args_t>{});
    } else {
        return apply_range_args(
            std::forward<Matcher>(matcher).handler,
            std::forward<Value>(v),
            arg_indexes_t<
                typename std::decay_t<Matcher>::pattern_type::args_t>{});
    }
}

template <std::size_t Index, typename Value, typename... Matchers>
constexpr decltype(auto) match_impl(Value&& v,
                                    const std::tuple<Matchers...>& matchers)
{
    auto& matcher     = std::get<Index>(matchers);
    using return_type = decltype(apply_value(matcher, std::forward<Value>(v)));

    if (matcher.pattern == v)
        return apply_value(matcher, std::forward<Value>(v));

    if constexpr (Index == sizeof...(Matchers) - 1) {
        if constexpr (std::is_void_v<return_type>)
            return;
        else
            DETAIL_CLO_UNREACHABLE();
    } else {
        return match_impl<Index + 1>(std::forward<Value>(v), matchers);
    }
}

template <typename Value, typename... Matchers>
constexpr bool valid_matchers_v =
    (std::is_void_v<decltype(apply_value(std::declval<Matchers>(),
                                         std::declval<Value>()))> &&
     ...) ||
    has_any_pattern_v<Matchers...> ||
    (is_detected_v<free_tied, Value> &&
     has_pattern_with_all_any_elements_v<Matchers...>);

template <typename Value, typename... Matchers>
constexpr decltype(auto) match(Value&& v,
                               const std::tuple<Matchers...>& matchers)
{
    static_assert(
        valid_matchers_v<Value, Matchers...>,
        "Either return void or have a case that covers all possibilities.");
    return match_impl<0>(std::forward<Value>(v), matchers);
}

template <typename Pattern, typename Handler>
struct case_holder
{
    using pattern_type = Pattern;
    using handler_type = Handler;

    pattern_type pattern;
    handler_type handler;
};

template <typename Pattern, typename Handler>
case_holder(Pattern&&, Handler &&)
    ->case_holder<std::decay_t<Pattern>, std::decay_t<Handler>>;

} // namespace detail

template <typename... PatternArgs>
struct case_
{
    using pattern_type = pattern<PatternArgs...>;
    constexpr case_(PatternArgs... args)
        : pattern{std::forward<PatternArgs>(args)...}
    {}
    pattern_type pattern;
};

namespace detail {

template <typename T>
constexpr bool is_case_v = false;

template <typename... Args>
constexpr bool is_case_v<case_<Args...>> = true;

} // namespace detail

constexpr struct default_t
{
    using pattern_type = any_pattern;
} default_{};

template <typename Case,
          typename Func,
          std::enable_if_t<detail::is_case_v<std::decay_t<Case>>>* = nullptr>
constexpr auto operator|=(Case&& c, Func&& f)
{
    return detail::case_holder{std::forward<Case>(c).pattern,
                               std::forward<Func>(f)};
}

template <typename... PatternArgs, typename Func>
constexpr auto operator|=(default_t, Func&& f)
{
    return detail::case_holder{default_t::pattern_type{},
                               std::forward<Func>(f)};
}

template <typename... CaseHolders>
constexpr auto make_matcher(CaseHolders&&... cases)
{
    if constexpr (sizeof...(CaseHolders) != 0) {
        return [cases = std::make_tuple(std::forward<CaseHolders>(cases)...)](
                   auto&& v) -> decltype(auto) {
            return detail::match(std::forward<decltype(v)>(v), cases);
        };
    } else {
        return [](auto&&) {};
    }
}

template <typename T>
constexpr decltype(auto) match(T&& v)
{
    return [v = std::forward<T>(v)](auto&&... cases) -> decltype(auto) {
        return make_matcher(std::forward<decltype(cases)>(cases)...)(v);
    };
}

} // namespace c_lo