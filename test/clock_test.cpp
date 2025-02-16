#include <gtest/gtest.h>

#include <clock.hpp>

namespace ink::test {

TEST(Clock, Empty) {
  Clock time{};
  auto [hour, minutes] = time.GetTime();
  EXPECT_EQ(hour, 0);
  EXPECT_EQ(minutes, 0);
}

TEST(Clock, Constructor) {
  Clock time_from_string{"03:59"};
  {
    auto [hour, minutes] = time_from_string.GetTime();
    EXPECT_EQ(hour, 3);
    EXPECT_EQ(minutes, 59);
  }

  Clock time_from_args{12, 5};
  {
    auto [hour, minutes] = time_from_args.GetTime();
    EXPECT_EQ(hour, 12);
    EXPECT_EQ(minutes, 5);
  }
}

TEST(Clock, ToString) {
  Clock time{2, 5};
  EXPECT_EQ(time.ToString(), "02:05");
}

TEST(Clock, Compare) {
  Clock time1{"12:30"};
  Clock time2{"23:59"};

  EXPECT_TRUE(time1 < time2);
}

TEST(Clock, Arithmetic) {
  Clock time{"12:42"};
  time += Clock{"03:20"};
  EXPECT_EQ(time.ToString(), "16:02");

  time = time - Clock{"16:01"};
  EXPECT_EQ(time.ToString(), "00:01");
}

}  // namespace ink::test
