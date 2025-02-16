#include <clock.hpp>

#include <stdexcept>

namespace ink {

Clock::Clock(const std::string& time_point_str)
    : hours(std::stoul(time_point_str.substr(0, 2))),
      minutes(std::stoul(time_point_str.substr(3, 2))) {}

Clock::Clock(size_t hours, size_t minutes) noexcept : hours(hours), minutes(minutes) {}

std::string Clock::ToString() const {
  const auto hours_str = (hours < 10) ? "0" + std::to_string(hours) : std::to_string(hours);
  const auto minutes_str = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
  return hours_str + ":" + minutes_str;
}

Clock Clock::operator+=(const Clock& other) noexcept {
  hours += other.hours;
  minutes += other.minutes;
  if (minutes >= 60) {
    ++hours;
    minutes -= 60;
  }
  return *this;
}

Clock operator-(const Clock& lhs, const Clock& rhs) {
  if (lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes)) {
    throw std::runtime_error{"Invalid time subtraction"};
  }

  Clock result = lhs;
  if (result.minutes < rhs.minutes) {
    --result.hours;
    result.minutes += 60;
  }

  result.hours -= rhs.hours;
  result.minutes -= rhs.minutes;

  return result;
}

}  // namespace ink
