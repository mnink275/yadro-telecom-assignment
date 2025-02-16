#include <fstream>
#include <iostream>
#include <string>
#include <optional>

#include <computer_club.hpp>
#include <utility.hpp>
#include <input.hpp>

namespace {

bool ReadLine(std::ifstream& input, std::string& line) {
  if (std::getline(input, line)) return true;

  std::cerr << "Unexpected end of file\n";
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: ./ComputerClubManager <filename.txt>\n";
    return 1;
  }

  std::string filename(argv[1]);
  std::ifstream input{filename};
  if (!input.is_open()) {
    std::cout << "Failed to open file '" + filename + "'\n";
    return 1;
  }

  std::string line;

  if (!ReadLine(input, line)) return 1;
  auto num_table_tokens = ink::input::ExpectInteger(line, 1);
  if (!num_table_tokens.has_value()) return 1;
  size_t num_tables = std::stoul(std::string{num_table_tokens.value()[0]});
  
  if (!ReadLine(input, line)) return 1;
  auto open_close_times_tokens = ink::input::ExpectTimePoint(line, 2);
  if (!open_close_times_tokens.has_value()) return 1;
  ink::Clock open_time{std::string{open_close_times_tokens.value()[0]}};
  ink::Clock close_time{std::string{open_close_times_tokens.value()[1]}};

  if (!ReadLine(input, line)) return 1;
  auto per_hour_price_tokens = ink::input::ExpectInteger(line, 1);
  if (!per_hour_price_tokens.has_value()) return 1;
  size_t per_hour_price = std::stoul(std::string{per_hour_price_tokens.value()[0]});

  std::queue<ink::Event> events;
  while (std::getline(input, line)) {
    auto event_tokens = ink::input::ExpectEvent(line);
    if (!event_tokens.has_value()) return 1;

    ink::Event event;
    const auto tokens = *event_tokens;
    event.time_point = ink::Clock{std::string{tokens[0]}};
    event.event_id = std::stoul(std::string{tokens[1]});
    event.client_name = std::string{tokens[2]};
    if (tokens.size() == 4) {
      event.table_id = std::stoul(std::string{tokens[3]});
      if (event.table_id > num_tables) {
        ink::input::PrintInvalidFormatAtLine(line);
        return 1;
      }
    }

    if (!events.empty() && event.time_point < events.front().time_point) {
      ink::input::PrintInvalidFormatAtLine(line);
      return 1;
    }

    events.emplace(std::move(event));
  }

  try {
    ink::ComputerClub computer_club_manager{num_tables, open_time, close_time,
                                          per_hour_price};
    while (!events.empty()) {
      auto event = std::move(events.front());
      events.pop();

      std::cout << event.time_point.ToString() << " ";
      std::cout << event.event_id << " ";
      std::cout << event.client_name << " ";
      if (event.event_id == 2) std::cout << event.table_id << " ";

      computer_club_manager.ProceedEvent(std::move(event));
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << '\n';
    return 1;
  }
}
