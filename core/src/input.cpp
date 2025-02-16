#include <input.hpp>

namespace ink::input {

MaybeTokens ValidateLine(const std::string& line, size_t count, Validator validator) {
  auto tokens = ink::utils::Split(line, ' ');
  if (tokens.size() != count || !validator(std::string{tokens[0]})) {
    PrintInvalidFormatAtLine(line);
    return std::nullopt;
  }

  return tokens;
}

MaybeTokens ValidateLineEvent(const std::string& line) {
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
