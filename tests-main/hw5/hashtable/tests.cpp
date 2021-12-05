#include "curricula.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#define protected public
#define private public
#include "Hashtable.h"
#undef protected
#undef private

TESTS_BEGIN

TEST(CheckHash_a)
{
	Hashtable<int> hashtable(true);
	int index{ hashtable.hash("a") };
	EXPECT_EQUAL(1, index % 11);
	PASS;
}

TEST(CheckHash_b)
{
	Hashtable<int> hashtable(true);
	int index{ hashtable.hash("b") };
	EXPECT_EQUAL(2, index % 11);
    PASS;
}

TEST(CheckHash_l)
{
	Hashtable<int> hashtable(true);
	int index{ hashtable.hash("l") };
	EXPECT_EQUAL(1, index % 11);
    PASS;
}

TEST(CheckHash_test)
{
	Hashtable<int> hashtable(true);
	int index{ hashtable.hash("test") };
    std::cout << index % 11 << std::endl;
	EXPECT_EQUAL(1, index % 11);
    PASS;
}

TEST(CheckHash_mediumlength)
{
	Hashtable<int> hashtable(true);
	int index{ hashtable.hash("mediumlength") };
    std::cout << index % 11 << std::endl;
	EXPECT_EQUAL(5, index % 11);
    PASS;
}

TEST(CheckHash_antidisestablishmentarianism)
{
	Hashtable<int> hashtable(true);
	int index{ hashtable.hash("antidisestablishmentarianism") };
	EXPECT_EQUAL(6, index % 11);
    PASS;
}

TEST(AddCheckCount_a)
{
	Hashtable<int> hashtable(true);
	hashtable.add("a", 1);
	EXPECT_EQUAL(1, hashtable.lookup("a"));
    PASS;
}

TEST(AddCheckCount_a_b)
{
	Hashtable<int> hashtable(true);
	hashtable.add("a", 69);
	hashtable.add("b", 420);
	EXPECT_EQUAL(69, hashtable.lookup("a"));
	EXPECT_EQUAL(420, hashtable.lookup("b"));
    PASS;
}

TEST(AddCheckCount_a_b_test)
{
	Hashtable<int> hashtable(true);
	hashtable.add("a", 1);
	hashtable.add("b", 2);
	hashtable.add("test", 3);
	EXPECT_EQUAL(1, hashtable.lookup("a"));
	EXPECT_EQUAL(2, hashtable.lookup("b"));
	EXPECT_EQUAL(3, hashtable.lookup("test"));
    PASS;
}

TEST(AddLinearProbingCheckCount_a_l)
{
	Hashtable<int> hashtable(true);
	hashtable.add("a", 1);
	// EXPECT_EQUAL(1, hashtable.hash("l") % 11);
	hashtable.add("l", 2);
	EXPECT_EQUAL(1, hashtable.lookup("a"));
	EXPECT_EQUAL(2, hashtable.lookup("l"));
	PASS;
}

TEST(AddLinearProbingCheckCount_a_aa_l)
{
	Hashtable<int> hashtable(true);
	hashtable.add("a", 1);
	// EXPECT_EQUAL(1, hashtable.hash("aa") % 11);
	hashtable.add("aa", 2);
	// EXPECT_EQUAL(2, hashtable.hash("l") % 11);
	hashtable.add("l", 3);
	EXPECT_EQUAL(1, hashtable.lookup("a"));
	EXPECT_EQUAL(2, hashtable.lookup("aa"));
	EXPECT_EQUAL(3, hashtable.lookup("l"));
    PASS;
}

TEST(ForceResize_Linear)
{
	Hashtable<int> hashtable(true);
	hashtable.add("a", 1);
	hashtable.add("b", 2);
	hashtable.add("c", 3);
	hashtable.add("d", 4);
	hashtable.add("e", 5);
	hashtable.add("f", 6);
	hashtable.add("g", 7);
	EXPECT_EQUAL(1, hashtable.lookup("a"));
	EXPECT_EQUAL(2, hashtable.lookup("b"));
	EXPECT_EQUAL(3, hashtable.lookup("c"));
	EXPECT_EQUAL(4, hashtable.lookup("d"));
	EXPECT_EQUAL(5, hashtable.lookup("e"));
	EXPECT_EQUAL(6, hashtable.lookup("f"));
	EXPECT_EQUAL(7, hashtable.lookup("g"));
	PASS;
}

TESTS_END
