#include <utility.hpp>

namespace ink::utils {

std::vector<std::string_view> Split(std::string_view str, char delimiter) {
  std::vector<std::string_view> result;
  size_t start = 0;
  while (start < str.size()) {
    const auto end = str.find(delimiter, start);
    if (end == std::string_view::npos) {
      result.push_back(str.substr(start));
      break;
    }

    const auto length = end - start;
    if (length > 0) {
      result.push_back(str.substr(start, end - start));
    }
    start = end + 1;
  }

  return result;
}

}  // namespace ink::utils
