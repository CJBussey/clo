#include <clo/match.hpp>
#include <clo/pattern.hpp>

#include <catch2/catch.hpp>

#include <tuple>

// LATER: use function_spy to clean up these tests
// rather than hand-writing these lambda spies

using static_test_args_t = std::tuple<clo::arg_t, int, clo::arg_t>;

static_assert(
    std::is_same_v<
        std::index_sequence<0, 2>,
        clo::detail::arg_indexes_t<static_test_args_t>
    >
);

struct TestStruct{ int i, j, k; };
auto as_tuple(const TestStruct& t)
{
    return std::tie(t.i, t.j, t.k);
}

TEST_CASE( "build_matcher: matching tuple with no args" )
{
    using namespace clo;

    SECTION( "one case, matching pattern, executes function" )
    {
        bool executed = false;

        auto match_ = build_matcher(
            case_{ _, _, _ } | [&](){ executed = true; }
        );
        match_(TestStruct{ 1, 2, 3 });

        CHECK( executed );
    }

    SECTION( "two cases, one matching pattern, executes function" )
    {
        bool executed = false;

        auto match_ = build_matcher(
            case_{ 2, 2, 1 } | [](){},
            case_{ 1, 2, 1 } | [&](){ executed = true; }
        );
        match_(TestStruct{1, 2, 1});

        CHECK( executed );
    }

    SECTION( "two cases, no matching pattern, function not executed" )
    {
        bool executed = false;

        auto match_ = build_matcher(
            case_{ 2, 2, 1 } | [&](){ executed = true; },
            case_{ 1, 2, 1 } | [&](){ executed = true; }
        );
        match_(TestStruct{666, 2, 1});

        CHECK( executed == false );
    }

    SECTION( "no cases, function not executed" )
    {
        bool executed = false;

        auto match_ = build_matcher();
        match_(TestStruct{666, 2, 1});

        CHECK( executed == false );
    }

    SECTION( "one case, return type, expected return is returned" )
    {
        auto match_ = build_matcher(
            case_{ _, _ } | []{ return 50; }
        );
        auto ret = match_(TestStruct{666, 2, 1});

        CHECK( ret == 50 );
    }
}

TEST_CASE( "build_matcher: matching tuple with args" )
{
    using namespace clo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = build_matcher(
            case_{ _, arg, _ } | [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(TestStruct{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = build_matcher(
            case_{ _, arg, arg } | [&](auto arg1, auto arg2){
                executed_with_correct_args = (arg1 == 2) && (arg2 == 3);
            }
        );
        match_(TestStruct{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "build_matcher: matching array with args" )
{
    using namespace clo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = build_matcher(
            case_{ _, arg, _ } | [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(std::array<int, 3>{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = build_matcher(
            case_{ _, arg, arg } | [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 3); }
        );
        match_(std::array<int, 3>{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "build_matcher: matching vector with args" )
{
    using namespace clo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = build_matcher(
            case_{ _, arg, _ } | [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(std::vector<int>{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = build_matcher(
            case_{ _, arg, arg } | [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 3); }
        );
        match_(std::vector<int>{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "match" )
{
    using namespace clo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        match(std::vector<int>{ 1, 2, 3 })(
            case_{ _, arg, _ } | [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        match(std::vector<int>{ 1, 2, 3 })(
            case_{ _, arg, arg } | [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 3); }
        );

        CHECK( executed_with_correct_args );
    }
}