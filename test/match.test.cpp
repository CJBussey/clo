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

TEST_CASE( "match: matching tuple with no args" )
{
    using namespace clo;

    SECTION( "one matcher, matching pattern, executes function" )
    {
        bool executed = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, _, _ }, [&](){ executed = true; } }
        );
        pattern_matcher(TestStruct{ 1, 2, 3 });

        CHECK( executed );
    }

    SECTION( "two matchers, one matching pattern, executes function" )
    {
        bool executed = false;

        auto pattern_matcher = match(
            matcher{ pattern{ 2, 2, 1 }, [](){} },
            matcher{ pattern{ 1, 2, 1 }, [&](){ executed = true; } }
        );
        pattern_matcher(TestStruct{1, 2, 1});

        CHECK( executed );
    }

    SECTION( "two matchers, no matching pattern, function not executed" )
    {
        bool executed = false;

        auto pattern_matcher = match(
            matcher{ pattern{ 2, 2, 1 }, [&](){ executed = true; } },
            matcher{ pattern{ 1, 2, 1 }, [&](){ executed = true; } }
        );
        pattern_matcher(TestStruct{666, 2, 1});

        CHECK( executed == false );
    }

    SECTION( "no matchers, function not executed" )
    {
        bool executed = false;

        auto pattern_matcher = match();
        pattern_matcher(TestStruct{666, 2, 1});

        CHECK( executed == false );
    }
}

TEST_CASE( "match: matching tuple with args" )
{
    using namespace clo;

    SECTION( "one matcher, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, arg, _ }, [&](auto arg){ executed_with_correct_arg = (arg == 2); } }
        );
        pattern_matcher(TestStruct{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one matcher, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, arg, arg }, [&](auto arg1, auto arg2){ 
                executed_with_correct_args = (arg1 == 2) && (arg2 == 3); 
            } }
        );
        pattern_matcher(TestStruct{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "match: matching array with args" )
{
    using namespace clo;

    SECTION( "one matcher, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, arg, _ }, [&](auto arg){ executed_with_correct_arg = (arg == 2); } }
        );
        pattern_matcher(std::array<int, 3>{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one matcher, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, arg, arg }, [&](auto arg1, auto arg2){ 
                executed_with_correct_args = (arg1 == 2) && (arg2 == 3); 
            } }
        );
        pattern_matcher(std::array<int, 3>{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "match: matching vector with args" )
{
    using namespace clo;

    SECTION( "one matcher, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, arg, _ }, [&](auto arg){ executed_with_correct_arg = (arg == 2); } }
        );
        pattern_matcher(std::vector<int>{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one matcher, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto pattern_matcher = match(
            matcher{ pattern{ _, arg, arg }, [&](auto arg1, auto arg2){ 
                executed_with_correct_args = (arg1 == 2) && (arg2 == 3); 
            } }
        );
        pattern_matcher(std::vector<int>{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}