// stage4/math/math_utils_test.cc
//
// cc_test is the Bazel rule for unit tests.
// It is like cc_binary but:
//   - 'bazel test' runs it and captures pass/fail
//   - results are cached: a test only reruns if its inputs change
//
// GoogleTest is declared as a dep via:
//   @googletest//:gtest_main
// The @googletest prefix refers to the external repo declared in MODULE.bazel.
// gtest_main provides main() — no need to write your own.

#include <gtest/gtest.h>
#include "stage4/math/math_utils.h"

// ---- add ----
TEST(MathTest, AddPositives) {
    EXPECT_EQ(math::add(2, 3), 5);
}

TEST(MathTest, AddNegatives) {
    EXPECT_EQ(math::add(-4, -6), -10);
}

TEST(MathTest, AddMixed) {
    EXPECT_EQ(math::add(10, -3), 7);
}

// ---- subtract ----
TEST(MathTest, Subtract) {
    EXPECT_EQ(math::subtract(10, 4), 6);
    EXPECT_EQ(math::subtract(0, 5), -5);
}

// ---- is_prime ----
TEST(MathTest, PrimeSmall) {
    EXPECT_FALSE(math::is_prime(0));
    EXPECT_FALSE(math::is_prime(1));
    EXPECT_TRUE(math::is_prime(2));
    EXPECT_TRUE(math::is_prime(3));
    EXPECT_FALSE(math::is_prime(4));
    EXPECT_TRUE(math::is_prime(5));
}

TEST(MathTest, PrimeLarger) {
    EXPECT_TRUE(math::is_prime(97));
    EXPECT_FALSE(math::is_prime(100));
}
