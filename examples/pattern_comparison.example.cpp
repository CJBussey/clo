#include <clo/match.hpp>
#include <clo/pattern.hpp>

#include <array>
#include <iostream>
#include <tuple>
#include <vector>

struct Numbers 
{
    int i, j, k, l;
};
auto as_tuple(const Numbers& n)
{
    return std::tie(n.i, n.j, n.k, n.l);
}

int main()
{
    using namespace clo;
    
    std::vector<int> vector              { 0, 1, 2, 3, 4 };
    std::array<int, 5> array             { 4, 3, 2, 1, 0 };
    std::tuple<int, int, int, int> tuple { 2, 2, 2, 2    };
    Numbers numbers                      { 1, 1, 1, 1    };
    
    pattern two_at_third_element { _, _, 2, _ };

    std::cout << "vector matches pattern  { _, _, 2, _ }: " << 
        (two_at_third_element == vector ? "true" : "false") << "\n";
    std::cout << "array matches pattern   { _, _, 2, _ }: " << 
        (two_at_third_element == array ? "true" : "false") << "\n"; 
    std::cout << "tuple matches pattern   { _, _, 2, _ }: " << 
        (two_at_third_element == tuple ? "true" : "false") << "\n"; 
    std::cout << "numbers matches pattern { _, _, 2, _ }: " << 
        (two_at_third_element == numbers ? "true" : "false") << "\n"; 
}