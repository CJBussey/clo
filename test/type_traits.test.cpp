#include <c-lo/detail/type_traits.hpp>

#include <array>

static_assert(
    c_lo::detail::is_array_v<
        std::array<int, 10>
    >
);

int main() {}