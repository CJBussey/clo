#include <clo/match.hpp>
#include <clo/pattern.hpp>

#include <iostream>
#include <string>
#include <tuple>

struct Person
{
    int age;
    std::string name;
};

auto as_tuple(const Person& p)
{
    return std::tie(p.age, p.name);
}

int main()
{
    using namespace clo;
    using namespace std::string_literals;

    auto print_me = [](){ std::cout << "That's me!\n"; };
    auto print_unknown = [](const auto& name){ 
        std::cout << "I didn't know about this person, but they are " << name << "\n";
    };

    auto pattern_matcher = match(
        matcher{ pattern{ _, "Carl Bussey"s }, print_me },
        matcher{ pattern{ _, arg            }, print_unknown }
    );

    pattern_matcher( Person{ 28,  "Carl Bussey"        } );
    pattern_matcher( Person{ 666, "wrapped in plastic" } );
}