#include <input.hpp>

#include <regex>

namespace ink::input {

namespace {

bool IsValidClientName(const std::string& name) {
  static const std::regex kClientNameRegex{"[a-z0-9_-]+"};
  return std::regex_match(name, kClientNameRegex);
}

bool IsValidInteger(const std::string& integer) {
  static const std::regex kIntegerRegex{"[0-9]+"};
  return std::regex_match(integer, kIntegerRegex);
}

bool IsValidTimePoint(const std::string& time_point) {
  static const std::regex kTimePointRegex{"(0[0-9]|1[0-9]|2[0-4]):([0-5][0-9])"};
  return std::regex_match(time_point, kTimePointRegex);
}

MaybeTokens ValidateLine(const std::string& line, size_t count, Validator validator) {
  auto tokens = ink::utils::Split(line, ' ');
  if (tokens.size() != count) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  for (const auto& token : tokens) {
    if (!validator(std::string{token})) {
      PrintInvalidFormatAtLine(line);
      return std::nullopt;
    }
  }

  return tokens;
}

}  // namespace

MaybeTokens ExpectInteger(const std::string& line, size_t count) {
  return ValidateLine(line, count, IsValidInteger);
}

MaybeTokens ExpectTimePoint(const std::string& line, size_t count) {
  return ValidateLine(line, count, IsValidTimePoint);
}


MaybeTokens ExpectEvent(const std::string& line) {
  auto tokens = ink::utils::Split(line, ' ');
  if (tokens.size() != 3 && tokens.size() != 4) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  static constexpr size_t kSpecialEventID = 2;
  if (tokens.size() == 4 && (std::stoul(std::string{tokens[1]}) != kSpecialEventID)) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  if (!IsValidTimePoint(std::string{tokens[0]})
      || !IsValidInteger(std::string{tokens[1]})
      || !IsValidClientName(std::string{tokens[2]})) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  if (tokens.size() == 4 && !IsValidInteger(std::string{tokens[3]})) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  return tokens;
}

}  // namespace ink::input
