#include <c-lo/match.hpp>

#include <iostream>
#include <string>
#include <tuple>

int main()
{
    using namespace c_lo;
    using namespace std::string_literals;

    match(std::make_tuple("Somebody Else"s))(
        case_{ "Carl Bussey"s   } |= []{ std::cout << "That's me!\n"; },
        case_{ "Somebody Else"s } |= []{ std::cout << "That is not me...\n"; },
        default_                  |= []{}
    );
}