#include <c-lo/match.hpp>
#include <c-lo/pattern.hpp>

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
    using namespace c_lo;
    using namespace std::string_literals;

    auto print_me = [](){ std::cout << "That's me!\n"; };
    auto print_unknown = [](const auto& name){
        std::cout << "I didn't know about this person, but they are " << name << "\n";
    };

    //---- Build and reuse the matcher

    auto match_ = make_matcher(
        case_{ _, "Carl Bussey"s } |= print_me,
        case_{ _, arg            } |= print_unknown
    );

    match_( Person{ 28,  "Carl Bussey"        } );
    match_( Person{ 666, "wrapped in plastic" } );

    //----- Immediately invoke a matcher

    match( Person{ 28, "Carl Bussey" } )(
        case_{ _, "Carl Bussey"s } |= print_me,
        case_{ _, arg            } |= print_unknown
    );
}