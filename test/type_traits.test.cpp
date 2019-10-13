#include <clo/detail/type_traits.hpp>

#include <array>

static_assert(
    clo::detail::is_array_v<
        std::array<int, 10>
    >
);

int main() {}