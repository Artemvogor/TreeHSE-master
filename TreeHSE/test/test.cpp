#include <doctest.h>
#include "../source/source.cpp"


TEST_CASE("Testing insertion")
{
    Veb<32> myVeb;
    myVeb.insert(1);
    myVeb.insert(2);
    myVeb.insert(3);
    REQUIRE(not myVeb.empty());
    REQUIRE((myVeb.get_max() == 3));
    REQUIRE((myVeb.get_min() == 1));
}

TEST_CASE("Testing find_next and lookup methods")
{
    Veb<32> myVeb;
    myVeb.insert(10);
    myVeb.insert(20);
    myVeb.insert(30);
    REQUIRE((myVeb.find_next(10) == 10));
    REQUIRE((myVeb.find_next(15) == 20));
    REQUIRE((myVeb.find_next(9) == 10));
    REQUIRE(myVeb.lookup(30));
    REQUIRE(not myVeb.lookup(15));
}

TEST_CASE("Complex testing")
{
    Veb<32> myVeb;
    myVeb.insert(5);
    myVeb.insert(35);
    myVeb.insert(10);
    REQUIRE((myVeb.empty() == false));
    REQUIRE((myVeb.get_min() == 5));
    REQUIRE((myVeb.find_next(2) == 5));
    myVeb.insert(2);
    REQUIRE((myVeb.get_min() == 2));
    REQUIRE((myVeb.find_next(2) == 2));
}

TEST_CASE("Debugging")
{
    Veb<3> myVeb;
    myVeb.insert(2);
    myVeb.insert(2);
    REQUIRE((myVeb.get_max() == myVeb.get_min() and myVeb.get_max() == 2));
    REQUIRE(myVeb.lookup(2));
}