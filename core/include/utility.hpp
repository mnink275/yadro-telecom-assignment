#pragma once

#include <vector>
#include <string_view>

namespace ink::utils {

std::vector<std::string_view> Split(std::string_view str, char delimiter);

}  // namespace ink::utils
