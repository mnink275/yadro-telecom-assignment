#include <gtest/gtest.h>

#include <input.hpp>

namespace ink::input::test {

TEST(InputValidation, ExpectInteger) {
  EXPECT_TRUE(ExpectInteger("1", 1));
  EXPECT_TRUE(ExpectInteger("1 2 3", 3));

  EXPECT_FALSE(ExpectInteger("1", 2));
  EXPECT_FALSE(ExpectInteger("1 2 3", 2));

  EXPECT_FALSE(ExpectInteger("a", 1));
  EXPECT_FALSE(ExpectInteger("1 2 a", 3));

  EXPECT_FALSE(ExpectInteger("", 1));
  EXPECT_FALSE(ExpectInteger(" ", 1));
  EXPECT_FALSE(ExpectInteger("   ", 1));
}

TEST(InputValidation, ExpectTimePoint) {
  EXPECT_TRUE(ExpectTimePoint("00:00", 1));
  EXPECT_TRUE(ExpectTimePoint("00:00 01:01", 2));

  EXPECT_FALSE(ExpectTimePoint("22:15", 2));
  EXPECT_FALSE(ExpectTimePoint("12:23 01:01", 1));

  EXPECT_FALSE(ExpectTimePoint("0:0", 1));
  EXPECT_FALSE(ExpectTimePoint("1:16 01:1", 2));

  EXPECT_FALSE(ExpectTimePoint("25:32", 1));
  EXPECT_FALSE(ExpectTimePoint("22:96", 1));

  EXPECT_FALSE(ExpectTimePoint("", 1));
  EXPECT_FALSE(ExpectTimePoint(" ", 1));
  EXPECT_FALSE(ExpectTimePoint("   ", 1));
}

TEST(InputValidation, ExpectEvent) {
  EXPECT_TRUE(ExpectEvent("00:00 1 name"));
  EXPECT_TRUE(ExpectEvent("00:00 2 name 1"));

  EXPECT_FALSE(ExpectEvent("00:00 1 name 1"));
  EXPECT_FALSE(ExpectEvent("00:00 1"));
  EXPECT_FALSE(ExpectEvent("00:00 1 name 1 2 3"));
  EXPECT_FALSE(ExpectEvent("erm 1 name"));
  EXPECT_FALSE(ExpectEvent("00:00 1 name+ 1"));
  EXPECT_FALSE(ExpectEvent("00:00 1 name 1 a"));
  EXPECT_FALSE(ExpectEvent(""));
}

}  // namespace ink::input::test
