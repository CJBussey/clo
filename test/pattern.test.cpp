#include <c-lo/pattern.hpp>

#include <catch2/catch.hpp>

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

TEST_CASE( "pattern: operator==(pattern, tuple)" ) {

    using namespace c_lo;

    SECTION( "pattern<any> and tuple<T> returns true" ) {
        auto sut = pattern{ _, _ };
        auto tuple = std::tuple<int, int>{ 1, 2 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<T> and tuple<T> with equal values returns true" ) {
        auto sut = pattern{ 666, 666 };
        auto tuple = std::tuple<int, int>{ 666, 666 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<T> and tuple<T> with at least one different value returns false" ) {
        auto sut = pattern{ 666, 666 };
        auto tuple = std::tuple<int, int>{ 0, 666 };

        CHECK( !(sut == tuple) );
    }

}

TEST_CASE( "pattern: operator==(pattern, array)" ) {

    using namespace c_lo;

    SECTION( "pattern<any> and array<T> returns true" ) {
        auto sut = pattern{ _, _ };
        auto tuple = std::array<int, 2>{ 1, 2 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<val> and array<T> with equal values returns true" ) {
        auto sut = pattern{ 666, 666 };
        auto tuple = std::array<int, 2>{ 666, 666 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<val> and array<T> with at least one different value returns false" ) {
        auto sut = pattern{ 666, 666 };
        auto tuple = std::array<int, 2>{ 0, 666 };

        CHECK( !(sut == tuple) );
    }

}

TEST_CASE( "pattern: operator==(pattern, vector)" ) {

    using namespace c_lo;

    SECTION( "pattern<any> and vector<T> returns true" ) {
        auto sut = pattern{ _, _ };
        auto tuple = std::vector<int>{ 1, 2 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<val> and vector<T> with equal values returns true" ) {
        auto sut = pattern{ 666, 666 };
        auto tuple = std::vector<int>{ 666, 666 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<val> and vector<T> with at least one different value returns false" ) {
        auto sut = pattern{ 666, 666 };
        auto tuple = std::vector<int>{ 0, 666 };

        CHECK( !(sut == tuple) );
    }

    SECTION( "pattern with no element true against empty vector" ) {
        auto sut = pattern{};
        auto tuple = std::vector<int>{};

        CHECK( sut == tuple );
    }

}

struct TestStruct { int i, j, k; };
auto tied(const TestStruct& s)
{
    return std::tie(s.i, s.j, s.k);
}

TEST_CASE( "pattern: operator==(pattern, struct)" ) {

    using namespace c_lo;

    SECTION( "pattern<any> and struct returns true" ) {
        auto sut = pattern{ _, _, _ };
        auto tuple = TestStruct{ 1, 2, 3 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<val> and struct with equal values returns true" ) {
        auto sut = pattern{ 666, 666, 666 };
        auto tuple = TestStruct{ 666, 666, 666 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern<val> and struct with at least one different values returns false" ) {
        auto sut = pattern{ 666, 1, 2 };
        auto tuple = TestStruct{ 0, 1, 2 };

        CHECK( !(sut == tuple) );
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

TEST_CASE( "pattern: operator==(pattern, range)" ) {

    using namespace c_lo;

    SECTION( "pattern<any> and struct returns true" ) {
        auto sut = pattern{ _, _, _ };
        auto v = std::vector{ 1, 2, 3 };
        auto r = v | reversed;

        CHECK( sut == r );
    }

    SECTION( "pattern<val> and struct with equal values returns true" ) {
        auto sut = pattern{ 666, 665, 664 };
        auto v = std::vector{ 664, 665, 666 };
        auto r = v | reversed;

        CHECK( sut == r );
    }

    SECTION( "pattern<val> and struct with at least one different values returns false" ) {
        auto sut = pattern{ 666, 1, 2 };
        auto v = std::vector{ 2, 1, 0 };
        auto r = v | reversed;

        CHECK( !(sut == r) );
    }
}

TEST_CASE( "nested patterns" ) {

    using namespace c_lo;

    SECTION( "pattern<pattern<any>> returns true for two dimensional vector" ) {
        auto sut = pattern{ pattern{_, _}, pattern{_, _} };
        auto v = std::vector{ std::vector{ 1, 2 }, std::vector{ 1, 2 } };

        CHECK( sut == v );
    }

    SECTION( "pattern<pattern<val>> returns true if matching two dimensional vector" ) {
        auto sut = pattern{ pattern{_, 2}, pattern{_, 2} };
        auto v = std::vector{ std::vector{ 1, 2 }, std::vector{ 1, 2 } };

        CHECK( sut == v );
    }
}