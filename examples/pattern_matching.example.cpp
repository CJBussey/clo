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

    auto match = build_matcher(
        case_{ pattern{ _, "Carl Bussey"s }, print_me },
        case_{ pattern{ _, arg            }, print_unknown }
    );

    match( Person{ 28,  "Carl Bussey"        } );
    match( Person{ 666, "wrapped in plastic" } );
}