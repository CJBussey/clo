#pragma once

#include <clo/as_tuple.hpp>
#include <clo/detail/meta.hpp>
#include <clo/pattern.hpp>

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>

namespace clo {

namespace detail {

template <typename T>
struct is_arg : std::false_type{};

template <>
struct is_arg<arg_t> : std::true_type{};

template <typename T>
constexpr bool is_arg_v = is_arg<T>::value;

template <typename Args, std::size_t Index>
struct arg_indexes;

template <typename Args>
struct arg_indexes<Args, 0>
{
    using type = append_to_index_sequence_if_t<0, std::index_sequence<>, is_arg_v<std::tuple_element_t<0, Args>>>;
};

template <typename Args, std::size_t Index>
struct arg_indexes
{
    using type = append_to_index_sequence_if_t<Index, typename arg_indexes<Args, Index - 1>::type, is_arg_v<std::tuple_element_t<Index, Args>>>;
};

template <typename Args>
using arg_indexes_t = typename arg_indexes<Args, std::tuple_size_v<Args> - 1>::type;

template <typename Func, typename ValueAsTuple, std::size_t ...Ns>
constexpr auto apply_tuple_args(Func&& func, ValueAsTuple&& value, std::index_sequence<Ns...>)
{
    return std::invoke(std::forward<Func>(func), std::get<Ns>(value)...);
}

template <typename Matcher, typename Value>
constexpr auto apply_args(Matcher&& matcher, Value&& value, try_t)
    CLO_RETURN(
        apply_tuple_args(matcher.handler, as_tuple(std::forward<Value>(value)),
                         arg_indexes_t<typename std::decay_t<Matcher>::pattern_type::args_t>{})
    )

template <typename Func, typename Range, std::size_t ...Ns>
constexpr auto apply_vector_args(Func&& func, Range&& r, std::index_sequence<Ns...>)
{
    return std::invoke(std::forward<Func>(func), get(r, Ns)...);
}

template <typename Matcher, typename Range>
constexpr auto apply_args(Matcher&& matcher, Range&& r, catch_t)
{
    return apply_vector_args(matcher.handler, std::forward<Range>(r),
                             arg_indexes_t<typename std::decay_t<Matcher>::pattern_type::args_t>{});
}

template <std::size_t Index, typename Value, typename MatcherTuple>
constexpr auto match_impl(Value&& v, MatcherTuple&& matchers)
{
    auto& matcher = std::get<Index>(matchers);
    using return_type = decltype(apply_args(matcher, std::forward<Value>(v), try_t{}));

    if (matcher.pattern == v)
        return apply_args(matcher, std::forward<Value>(v), try_t{});

    if constexpr (Index == std::tuple_size_v<std::decay_t<MatcherTuple>> - 1)
    {
        if constexpr (std::is_void_v<return_type>)
            return;
        else
            return return_type{};
    }
    else
    {
        return match_impl<Index + 1>(std::forward<Value>(v), std::forward<MatcherTuple>(matchers));
    }
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
case_holder(Pattern&&, Handler&&) -> case_holder<std::decay_t<Pattern>, std::decay_t<Handler>>;

}

template <typename ...PatternArgs>
struct case_
{
    using pattern_type = pattern<PatternArgs...>;
    case_(PatternArgs... args) : pattern{std::forward<PatternArgs>(args)...} {}
    pattern_type pattern;
};

template <typename Case, typename Func>
constexpr auto operator|(Case&& c, Func&& f)
    CLO_RETURN(( detail::case_holder{ std::forward<typename Case::pattern_type>(c.pattern), std::forward<Func>(f) } ))

template <typename ...CaseHolders>
constexpr auto make_matcher(CaseHolders&&... cases)
{
    if constexpr (sizeof...(CaseHolders) != 0)
    {
        return [cases = std::make_tuple(cases...)](auto&& v) {
            return detail::match_impl<0>(std::forward<decltype(v)>(v), cases);
        };
    }
    else
    {
        return [](auto&&){};
    }
}

template <typename T>
constexpr auto match(T&& v)
{
    return [v = std::forward<T>(v)](auto&&... cases) {
        return make_matcher(std::forward<decltype(cases)>(cases)...)(v);
    };
}


}