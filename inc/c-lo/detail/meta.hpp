#pragma once

#include <cstddef>
#include <utility>

#define CLO_RETURN( exp )                                              \
        noexcept( noexcept( exp ) ) -> decltype( exp ) { return exp; } \
        /**/

namespace c_lo::detail {

template <std::size_t N>
struct meta : meta<N-1> {};

template <>
struct meta<0> {};

using try_t = meta<1>;
using catch_t = meta<0>;



template <std::size_t Index, typename IndexSequence>
struct append_to_index_sequence;

template <std::size_t Index, std::size_t ...Indexes>
struct append_to_index_sequence<Index, std::index_sequence<Indexes...>>
{
    using type = std::index_sequence<Indexes..., Index>;   
};

template <typename std::size_t Index, typename IndexSequence>
using append_to_index_sequence_t = typename append_to_index_sequence<Index, IndexSequence>::type;

template <std::size_t Index, typename IndexSequence, bool>
struct append_to_index_sequence_if;

template <std::size_t Index, typename IndexSequence>
struct append_to_index_sequence_if<Index, IndexSequence, true> 
{
    using type = typename append_to_index_sequence<Index, IndexSequence>::type;
};

template <std::size_t Index, typename IndexSequence>
struct append_to_index_sequence_if<Index, IndexSequence, false>
{
    using type = IndexSequence;
};

template <typename std::size_t Index, typename IndexSequence, bool Append>
using append_to_index_sequence_if_t = typename append_to_index_sequence_if<Index, IndexSequence, Append>::type;

}