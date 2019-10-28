#include <c-lo/match.hpp>
#include <c-lo/pattern.hpp>

#include <catch2/catch.hpp>

#include <tuple>

// LATER: use function_spy to clean up these tests
// rather than hand-writing these lambda spies

using static_test_args_t = std::tuple<c_lo::arg_t, int, c_lo::arg_t>;

static_assert(
    std::is_same_v<
        std::index_sequence<0, 2>,
        c_lo::detail::arg_indexes_t<static_test_args_t>
    >
);

struct TestStruct{ int i, j, k; };
auto tied(const TestStruct& t)
{
    return std::tie(t.i, t.j, t.k);
}

TEST_CASE( "make_matcher: matching tuple with no args" )
{
    using namespace c_lo;

    SECTION( "one case, matching pattern, executes function" )
    {
        bool executed = false;

        auto match_ = make_matcher(
            case_{ _, _, _ } |= [&]{ executed = true; }
        );
        match_(TestStruct{ 1, 2, 3 });

        CHECK( executed );
    }

    SECTION( "two cases, one matching pattern, executes function" )
    {
        bool executed = false;

        auto match_ = make_matcher(
            case_{ 2, 2, 1 } |= []{},
            case_{ 1, 2, 1 } |= [&]{ executed = true; }
        );
        match_(TestStruct{1, 2, 1});

        CHECK( executed );
    }

    SECTION( "two cases, no matching pattern, function not executed" )
    {
        bool executed = false;

        auto match_ = make_matcher(
            case_{ 2, 2, 1 } |= [&]{ executed = true; },
            case_{ 1, 2, 1 } |= [&]{ executed = true; }
        );
        match_(TestStruct{666, 2, 1});

        CHECK( executed == false );
    }

    SECTION( "no cases, function not executed" )
    {
        bool executed = false;

        auto match_ = make_matcher();
        match_(TestStruct{666, 2, 1});

        CHECK( executed == false );
    }

    SECTION( "one case, return type, expected return is returned" )
    {
        auto match_ = make_matcher(
            case_{ _, _, _ } |= []{ return 50; }
        );
        auto ret = match_(TestStruct{666, 2, 7});

        CHECK( ret == 50 );
    }
}

TEST_CASE( "make_matcher: matching tuple with args" )
{
    using namespace c_lo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = make_matcher(
            case_{ _, arg, _ } |= [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(TestStruct{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = make_matcher(
            case_{ _, arg, arg } |= [&](auto arg1, auto arg2){
                executed_with_correct_args = (arg1 == 2) && (arg2 == 3);
            }
        );
        match_(TestStruct{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "make_matcher: matching array with args" )
{
    using namespace c_lo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = make_matcher(
            case_{ _, arg, _ } |= [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(std::array<int, 3>{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = make_matcher(
            case_{ _, arg, arg } |= [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 3); }
        );
        match_(std::array<int, 3>{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "make_matcher: matching vector with args" )
{
    using namespace c_lo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = make_matcher(
            case_{ _, arg, _ } |= [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(std::vector<int>{ 1, 2, 3 });

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = make_matcher(
            case_{ _, arg, arg } |= [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 3); }
        );
        match_(std::vector<int>{ 1, 2, 3 });

        CHECK( executed_with_correct_args );
    }
}

template <typename Range>
struct reverse_holder
{
    auto begin() const { return r.rbegin(); }
    auto cbegin() const { return r.crbegin(); }
    auto end() const { return r.rend(); }
    auto cend() const { return r.crend(); }
    Range r;
};

struct reverse_t {} reversed;

template <typename Range>
auto operator|(Range&& r, reverse_t)
{
    return reverse_holder<std::decay_t<Range>>{std::forward<Range>(r)};
}

TEST_CASE( "make_matcher: matching range with args" )
{
    using namespace c_lo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        auto match_ = make_matcher(
            case_{ _, arg, _ } |= [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );
        match_(std::vector<int>{ 1, 2, 3 } | reversed);

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        auto match_ = make_matcher(
            case_{ _, arg, arg } |= [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 1); }
        );
        match_(std::vector<int>{ 1, 2, 3 } | reversed);

        CHECK( executed_with_correct_args );
    }
}

TEST_CASE( "match" )
{
    using namespace c_lo;

    SECTION( "one case, matching pattern, executes function with correct arg" )
    {
        bool executed_with_correct_arg = false;

        match(std::vector<int>{ 1, 2, 3 })(
            case_{ _, arg, _ } |= [&](auto arg){ executed_with_correct_arg = (arg == 2); }
        );

        CHECK( executed_with_correct_arg );
    }

    SECTION( "one case, matching pattern, executes function with multiple args with correct args" )
    {
        bool executed_with_correct_args = false;

        match(std::vector<int>{ 1, 2, 3 })(
            case_{ _, arg, arg } |= [&](auto arg1, auto arg2){ executed_with_correct_args = (arg1 == 2) && (arg2 == 3); }
        );

        CHECK( executed_with_correct_args );
    }

    struct non_default{ non_default(int){} };

    SECTION( "non-default return possible with default case" )
    {
        auto d = []{ return non_default{5}; };
        static_assert(detail::is_any_pattern_v<std::decay_t<std::decay_t<decltype(default_)>::pattern_type>>);
        static_assert(detail::has_any_pattern_v<decltype(operator|=(default_, d))>);

        auto result = match(std::vector<int>{ 1, 2, 3 })(
            case_{ _, _, _ } |= [&]{ return non_default{10}; },
            default_         |= [] { return non_default{5};  }
        ); // should compile
        std::ignore = result;
    }

    SECTION( "supports returning reference" )
    {
        const int i = 666;
        const auto& result = match(std::make_tuple(10))(
            case_{ _ } |= [&]() -> decltype(auto) { return std::forward<const int>(i); }
        );
        CHECK( &i == &result );
    }
}
