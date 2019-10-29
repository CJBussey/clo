#pragma once

#include <c-lo/detail/meta.hpp>

#include <cstddef>
#include <iterator>

namespace c_lo::detail {

template <typename Range>
constexpr auto size(const Range& r, try_t) CLO_RETURN(std::size(r))

    template <typename Range>
    constexpr auto size(const Range& r, catch_t)
        CLO_RETURN(std::end(r) - std::begin(r))

            template <typename Range>
            constexpr auto size(const Range& r) CLO_RETURN(size(r, try_t{}))

                template <typename Range>
                constexpr auto& get(Range& r, size_t index)
{
    auto it = std::begin(r);
    std::advance(it, index);
    return *it;
}

} // namespace c_lo::detail