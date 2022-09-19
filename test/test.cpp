#include "../vebtree.cpp"
#include <doctest.h>


TEST_CASE("Testing insertion/deletion process")
{
	Veb myVeb(10);
	REQUIRE(myVeb.empty());
	myVeb.Add(1);
	myVeb.Add(2);
	REQUIRE((myVeb.GetMax() == 2) and (myVeb.GetMin() == 1);
	myVeb.Remove(2);
	myVeb.Remove(1);
	REQUIRE(myVeb.empty());
}

TEST_CASE("Testing finding next and previous element")
{
	Veb myVeb(10);
	myVeb.Add(1);
	myVeb.Add(3);
	REQUIRE(myVeb.FindNext(2) == 3);
	REQUIRE(myVeb.FindPrev(2) == 1);
	REQUIRE(myVeb.myVeb.FindNext(1) == 1);
	REQUIRE(myVeb.myVeb.FindPrev(3) == 3);
}

TEST_CASE("Complex testing")
{
	Veb myVeb(10);
	myVeb.Add(5);
	REQUIRE(myVeb.empty() == false);
	myVeb.Add(10);
	myVeb.Add(20);
	REQUIRE(myVeb.FindNext(7) == 10);
	REQUIRE(myVeb.FindPrev(12) == 10);
	REQUIRE(myVeb.Lookup(5));
	REQUIRE(myVeb.Lookup(7) == false);
	myVeb.Remove(10);
	REQUIRE(myVeb.Lookup(10) == false);
	REQUIRE(myVeb.FindNext(7) == 20);
}

TEST_CASE("Testing wrong data handling")
{
	Veb myVeb(5);
	myVeb.Add(100);
	myVeb.Add(101);
	REQUIRE(true);
	myVeb.Add(10);
	myVeb.Add(10);
	REQUIRE(true);
	myVeb.Remove(10);
	myVeb.Remove(10);
	REQUIRE(true);
}