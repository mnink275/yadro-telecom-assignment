#include <gtest/gtest.h>

#include <input.hpp>

namespace ink::utils::test {

TEST(Utilities, Split) {
  const std::string_view str = "  a b c d e ";
  const auto result = Split(str, ' ');

  EXPECT_EQ(result.size(), 5);
  for (size_t i = 0; i < 5; ++i) {
    const auto ch = std::string(1, 'a' + i);
    EXPECT_EQ(result[i], std::string_view{ch});
  }
}

TEST(Utilities, SplitAnotherDelimiter) {
  const std::string_view str = "Hello,World,!";
  const auto result = Split(str, ',');

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "Hello");
  EXPECT_EQ(result[1], "World");
  EXPECT_EQ(result[2], "!");
}

}  // namespace ink::utils::test
