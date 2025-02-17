#include <gtest/gtest.h>

#include <computer_club.hpp>

namespace ink::test {

TEST(ComputerClub, NotOpenYet) {
  ComputerClub club{3, Clock{"07:00"}, Clock{"23:30"}, 100};

  try {
    club.ProceedEvent({Clock{"06:00"}, 1, "client1"});
    FAIL() << "Expected EventException";
  } catch (const EventException& ev_ex) {
    EXPECT_STREQ(ev_ex.what(), "06:00 13 NotOpenYet");
  } catch (...) {
    FAIL() << "Expected EventException, but caught a different exception";
  }

  try {
    club.ProceedEvent({Clock{"23:59"}, 1, "client1"});
    FAIL() << "Expected EventException";
  } catch (const EventException& ev_ex) {
    EXPECT_STREQ(ev_ex.what(), "23:59 13 NotOpenYet");
  } catch (...) {
    FAIL() << "Expected EventException, but caught a different exception";
  }
}

TEST(ComputerClub, YouShallNotPass) {
  ComputerClub club{3, Clock{"07:00"}, Clock{"23:30"}, 100};

  try {
    club.ProceedEvent({Clock{"12:00"}, 1, "client1"});

    club.ProceedEvent({Clock{"13:00"}, 1, "client1"});
    FAIL() << "Expected EventException";
  } catch (const EventException& ev_ex) {
    EXPECT_STREQ(ev_ex.what(), "13:00 13 YouShallNotPass");
  } catch (...) {
    FAIL() << "Expected EventException, but caught a different exception";
  }
}

TEST(ComputerClub, ClientUnknown) {
  ComputerClub club{3, Clock{"07:00"}, Clock{"23:30"}, 100};

  try {
    club.ProceedEvent({Clock{"12:00"}, 2, "client1", 1});
    FAIL() << "Expected EventException";
  } catch (const EventException& ev_ex) {
    EXPECT_STREQ(ev_ex.what(), "12:00 13 ClientUnknown");
  } catch (...) {
    FAIL() << "Expected EventException, but caught a different exception";
  }
}

TEST(ComputerClub, ICanWaitNoLonger) {
  ComputerClub club{1, Clock{"07:00"}, Clock{"23:30"}, 100};

  try {
    club.ProceedEvent({Clock{"12:00"}, 1, "client1"});
    club.ProceedEvent({Clock{"12:10"}, 3, "client1"});
    FAIL() << "Expected EventException";
  } catch (const EventException& ev_ex) {
    EXPECT_STREQ(ev_ex.what(), "12:10 13 ICanWaitNoLonger!");
  } catch (...) {
    FAIL() << "Expected EventException, but caught a different exception";
  }
}

TEST(ComputerClub, PlaceIsBusy) {
  ComputerClub club{1, Clock{"07:00"}, Clock{"23:30"}, 100};

  try {
    EXPECT_NO_THROW(club.ProceedEvent({Clock{"12:00"}, 1, "client1"}));
    EXPECT_NO_THROW(club.ProceedEvent({Clock{"12:10"}, 2, "client1", 1}));

    EXPECT_NO_THROW(club.ProceedEvent({Clock{"12:20"}, 1, "client2"}));
    club.ProceedEvent({Clock{"12:30"}, 2, "client2", 1});
    FAIL() << "Expected EventException";
  } catch (const EventException& ev_ex) {
    EXPECT_STREQ(ev_ex.what(), "12:30 13 PlaceIsBusy");
  } catch (...) {
    FAIL() << "Expected EventException, but caught a different exception";
  }
}

}  // namespace ink::test
