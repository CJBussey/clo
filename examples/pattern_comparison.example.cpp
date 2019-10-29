#include <c-lo/pattern.hpp>

#include <array>
#include <iostream>
#include <tuple>
#include <vector>

struct Numbers
{
    int i, j, k, l;
};
auto tied(const Numbers& n) { return std::tie(n.i, n.j, n.k, n.l); }

#define CHECK(__expression__)                                                  \
    std::cout << #__expression__ << "\t"                                       \
              << (__expression__ ? "PASSED" : "FAILED") << "\n"

int main()
{
    using namespace c_lo;

    std::vector vector{0, 1, 2, 3, 4};
    CHECK((vector == pattern{_, _, 2, _, _}));

    std::array<int, 5> array{4, 3, 2, 1, 0};
    CHECK((array == pattern{_, 3, _, _, _}));

    std::tuple<int, int, int, int> tuple{2, 2, 2, 666};
    CHECK((tuple == pattern{2, 2, 2, _}));

    Numbers numbers{1, 1, 1, 1};
    CHECK((numbers != pattern{666, _, _, _}));

    std::vector vector2{1, 2, 3};
    CHECK((numbers == pattern{666, _, _, _})); // fails test
}