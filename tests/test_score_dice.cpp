#include "ScoreDice.h"
#include <cstdio>
#include <cstdlib>

static int failures = 0;

#define EXPECT_EQ(expr, expected) do { \
	auto actual = (expr); \
	if (actual != (expected)) { \
		std::fprintf(stderr, "FAIL %s:%d: %s == %d, expected %d\n", \
			__FILE__, __LINE__, #expr, (int)actual, (int)(expected)); \
		++failures; \
	} \
} while (0)

static ScoreDice make(short a, short b, short c, short d, short e)
{
	short dice[5] = {a, b, c, d, e};
	return ScoreDice(dice);
}

static void test_upper_section()
{
	auto sd = make(1, 2, 3, 4, 5);
	EXPECT_EQ(sd.Aces(), 1);
	EXPECT_EQ(sd.Twos(), 2);
	EXPECT_EQ(sd.Threes(), 3);
	EXPECT_EQ(sd.Fours(), 4);
	EXPECT_EQ(sd.Fives(), 5);
	EXPECT_EQ(sd.Sixes(), 0);

	sd = make(3, 3, 3, 6, 6);
	EXPECT_EQ(sd.Threes(), 9);
	EXPECT_EQ(sd.Sixes(), 12);
	EXPECT_EQ(sd.Aces(), 0);

	sd = make(6, 6, 6, 6, 6);
	EXPECT_EQ(sd.Sixes(), 30);
	EXPECT_EQ(sd.Fives(), 0);
}

static void test_three_of_a_kind()
{
	auto sd = make(3, 3, 3, 4, 5);
	EXPECT_EQ(sd.ThreeOfAKind(), 18);

	sd = make(1, 2, 3, 4, 5);
	EXPECT_EQ(sd.ThreeOfAKind(), 0);

	// Four of a kind also qualifies
	sd = make(2, 2, 2, 2, 5);
	EXPECT_EQ(sd.ThreeOfAKind(), 13);

	// Yahtzee also qualifies
	sd = make(4, 4, 4, 4, 4);
	EXPECT_EQ(sd.ThreeOfAKind(), 20);
}

static void test_four_of_a_kind()
{
	auto sd = make(2, 2, 2, 2, 5);
	EXPECT_EQ(sd.FourOfAKind(), 13);

	sd = make(3, 3, 3, 4, 5);
	EXPECT_EQ(sd.FourOfAKind(), 0);

	// Yahtzee also qualifies
	sd = make(5, 5, 5, 5, 5);
	EXPECT_EQ(sd.FourOfAKind(), 25);
}

static void test_full_house()
{
	auto sd = make(2, 2, 3, 3, 3);
	EXPECT_EQ(sd.FullHouse(), 25);

	sd = make(1, 1, 2, 2, 3);
	EXPECT_EQ(sd.FullHouse(), 0);

	// Yahtzee is NOT a full house (no pair separate from triple)
	sd = make(4, 4, 4, 4, 4);
	EXPECT_EQ(sd.FullHouse(), 0);
}

static void test_small_sequence()
{
	// 1-2-3-4
	auto sd = make(1, 2, 3, 4, 4);
	EXPECT_EQ(sd.SmallSequence(), 30);

	// 2-3-4-5
	sd = make(2, 3, 4, 5, 5);
	EXPECT_EQ(sd.SmallSequence(), 30);

	// 3-4-5-6
	sd = make(1, 3, 4, 5, 6);
	EXPECT_EQ(sd.SmallSequence(), 30);

	sd = make(1, 2, 4, 5, 6);
	EXPECT_EQ(sd.SmallSequence(), 0);
}

static void test_large_sequence()
{
	auto sd = make(1, 2, 3, 4, 5);
	EXPECT_EQ(sd.LargeSequence(), 40);

	sd = make(2, 3, 4, 5, 6);
	EXPECT_EQ(sd.LargeSequence(), 40);

	sd = make(1, 2, 3, 4, 4);
	EXPECT_EQ(sd.LargeSequence(), 0);
}

static void test_yahtzee()
{
	auto sd = make(3, 3, 3, 3, 3);
	EXPECT_EQ(sd.Yahtzee(), 50);
	EXPECT_EQ(sd.IsYahtzee(), true);

	sd = make(3, 3, 3, 3, 4);
	EXPECT_EQ(sd.Yahtzee(), 0);
	EXPECT_EQ(sd.IsYahtzee(), false);
}

static void test_chance()
{
	auto sd = make(1, 2, 3, 4, 5);
	EXPECT_EQ(sd.Chance(), 15);

	sd = make(6, 6, 6, 6, 6);
	EXPECT_EQ(sd.Chance(), 30);

	sd = make(1, 1, 1, 1, 1);
	EXPECT_EQ(sd.Chance(), 5);
}

static void test_yahtzee_joker()
{
	// Yahtzee joker: all lower section categories score as if qualified
	auto sd = make(4, 4, 4, 4, 4);
	sd.SetYahtzeeJoker(true);

	EXPECT_EQ(sd.FullHouse(), 25);
	EXPECT_EQ(sd.SmallSequence(), 30);
	EXPECT_EQ(sd.LargeSequence(), 40);
}

int main()
{
	test_upper_section();
	test_three_of_a_kind();
	test_four_of_a_kind();
	test_full_house();
	test_small_sequence();
	test_large_sequence();
	test_yahtzee();
	test_chance();
	test_yahtzee_joker();

	if (failures) {
		std::fprintf(stderr, "%d test(s) FAILED\n", failures);
		return 1;
	}
	std::puts("All tests passed.");
	return 0;
}
