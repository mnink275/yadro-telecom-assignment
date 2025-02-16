#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <optional>

#include <utility.hpp>

namespace ink::input {

inline void PrintInvalidFormatAtLine(const std::string& line) {
  std::cerr << "Invalid input format at the line:\n";
  std::cerr << line << '\n';
}

using MaybeTokens = std::optional<std::vector<std::string_view>>;
using Validator = bool(*)(const std::string&);

MaybeTokens ExpectInteger(const std::string& line, size_t count);

MaybeTokens ExpectTimePoint(const std::string& line, size_t count);

MaybeTokens ExpectEvent(const std::string& line);

}  // namespace ink::input
