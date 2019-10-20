#include <c-lo/detail/meta.hpp>

#include <utility>
#include <type_traits>

static_assert(
    std::is_same_v<
        std::index_sequence<1, 2, 3>,
        c_lo::detail::append_to_index_sequence_t<3, std::index_sequence<1, 2>>
    >);

static_assert(
    std::is_same_v<
        std::index_sequence<1, 2, 3>,
        c_lo::detail::append_to_index_sequence_if_t<3, std::index_sequence<1, 2>, true>
    >);

static_assert(
    std::is_same_v<
        std::index_sequence<1, 2>,
        c_lo::detail::append_to_index_sequence_if_t<3, std::index_sequence<1, 2>, false>
    >);

int main() {}