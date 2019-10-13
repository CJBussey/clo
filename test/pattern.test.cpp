#include <clo/pattern.hpp>

#include <catch2/catch.hpp>

#include <array>
#include <tuple>
#include <vector>

TEST_CASE( "pattern: operator==(pattern, tuple)" ) {

    using namespace clo;

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

    SECTION( "pattern with last element any matches true against rest of tuple elements" ) {
        auto sut = pattern{ _ };
        auto tuple = std::tuple<int, double, bool>{ 1, 2., false };

        CHECK( sut == tuple );
    }

}

TEST_CASE( "pattern: operator==(pattern, array)" ) {

    using namespace clo;

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

    SECTION( "pattern with last element any matches true against rest of tuple elements" ) {
        auto sut = pattern{ _ };
        auto tuple = std::array<int, 3>{ 1, 2, 3 };

        CHECK( sut == tuple );
    }

}

TEST_CASE( "pattern: operator==(pattern, vector)" ) {

    using namespace clo;

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

    SECTION( "pattern with last element any matches true against rest of tuple elements" ) {
        auto sut = pattern{ _ };
        auto tuple = std::vector<int>{ 1, 2, 3 };

        CHECK( sut == tuple );
    }

    SECTION( "pattern with no element true against empty vector" ) {
        auto sut = pattern{};
        auto tuple = std::vector<int>{};

        CHECK( sut == tuple );
    }

}

struct TestStruct { int i, j, k; };
auto as_tuple(const TestStruct& s)
{
    return std::tie(s.i, s.j, s.k);
}

TEST_CASE( "pattern: operator==(pattern, struct)" ) {

    using namespace clo;

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

    SECTION( "pattern with last element any matches true against rest of tuple elements" ) {
        auto sut = pattern{ _ };
        auto tuple = TestStruct{ 1, 2, 3 };

        CHECK( sut == tuple );
    }
}