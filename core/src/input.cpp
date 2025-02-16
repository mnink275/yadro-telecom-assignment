#include <input.hpp>

#include <regex>

namespace ink::input {

namespace {

bool IsClientNameValid(const std::string& name) {
  static const std::regex kClientNameRegex{"[a-z0-9_-]+"};
  return std::regex_match(name, kClientNameRegex);
}

bool IsIntegerValid(const std::string& integer) {
  static const std::regex kIntegerRegex{"[0-9]+"};
  return std::regex_match(integer, kIntegerRegex);
}

bool IsTimePointValid(const std::string& time_point) {
  static const std::regex kTimePointRegex{"[0-9]{2}:[0-9]{2}"};
  return std::regex_match(time_point, kTimePointRegex);
}

MaybeTokens ValidateLine(const std::string& line, size_t count, Validator validator) {
  auto tokens = ink::utils::Split(line, ' ');
  if (tokens.size() != count || !validator(std::string{tokens[0]})) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  return tokens;
}

}  // namespace

MaybeTokens ExpectInteger(const std::string& line, size_t count) {
  return ValidateLine(line, count, IsIntegerValid);
}

MaybeTokens ExpectTimePoint(const std::string& line, size_t count) {
  return ValidateLine(line, count, IsTimePointValid);
}


MaybeTokens ExpectEvent(const std::string& line) {
  auto tokens = ink::utils::Split(line, ' ');
  if (tokens.size() != 3 && tokens.size() != 4) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  if (!IsTimePointValid(std::string{tokens[0]})
      || !IsIntegerValid(std::string{tokens[1]})
      || !IsClientNameValid(std::string{tokens[2]})) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  if (tokens.size() == 4 && !IsIntegerValid(std::string{tokens[3]})) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  return tokens;
}

}  // namespace ink::input
