#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <optional>
#include <regex>

#include <utility.hpp>

namespace ink::input {

inline void PrintInvalidFormatAtLine(const std::string& line) {
  std::cerr << "Invalid input format at the line:\n";
  std::cerr << line << '\n';
}

inline bool IsClientNameValid(const std::string& name) {
  static const std::regex kClientNameRegex{"[a-z0-9_-]+"};
  return std::regex_match(name, kClientNameRegex);
}

inline bool IsIntegerValid(const std::string& integer) {
  static const std::regex kIntegerRegex{"[0-9]+"};
  return std::regex_match(integer, kIntegerRegex);
}

inline bool IsTimePointValid(const std::string& time_point) {
  static const std::regex kTimePointRegex{"[0-9]{2}:[0-9]{2}"};
  return std::regex_match(time_point, kTimePointRegex);
}

using MaybeTokens = std::optional<std::vector<std::string_view>>;
using Validator = bool(*)(const std::string&);

MaybeTokens ValidateLine(const std::string& line, size_t count, Validator validator);

inline MaybeTokens ExpectInteger(const std::string& line, size_t count) {
  return ValidateLine(line, count, IsIntegerValid);
}

inline MaybeTokens ExpectTimePoint(const std::string& line, size_t count) {
  return ValidateLine(line, count, IsTimePointValid);
}

MaybeTokens ValidateLineEvent(const std::string& line);

}  // namespace ink::input
