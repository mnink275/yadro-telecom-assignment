#pragma once

#include <cstddef>
#include <string>

namespace ink {

class Clock {
 public:
  Clock() = default;

  explicit Clock(const std::string& time_point_str);

  Clock(size_t hours, size_t minutes);

  std::string ToString() const;
  std::pair<size_t, size_t> GetTime() const { return {hours, minutes}; }

  bool operator<(const Clock& other) {
    return std::tie(hours, minutes) < std::tie(other.hours, other.minutes);
  }

  Clock operator+=(const Clock& other);

  friend Clock operator-(const Clock& lhs, const Clock& rhs);

 private:
  size_t hours{0};
  size_t minutes{0};
};

}  // namespace ink
