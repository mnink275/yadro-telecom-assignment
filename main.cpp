#include <fstream>
#include <iostream>
#include <string>

#include <computer_club.hpp>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Run program with './app filename.txt'\n";
    return 1;
  }

  std::string filename(argv[1]);
  std::ifstream input{filename};
  if (!input.is_open()) {
    std::cout << "Failed to open file '" + filename + "'\n";
    return 1;
  }

  size_t num_tables{};
  input >> num_tables;

  std::string time_point;
  input >> time_point;
  ink::Clock open_time{time_point};
  input >> time_point;
  ink::Clock close_time{time_point};

  size_t per_hour_price{};
  input >> per_hour_price;

  ink::ComputerClub computer_club_manager{num_tables, open_time, close_time,
                                          per_hour_price};

  while (input >> time_point) {
    if (time_point == "15:52") {
      std::cout << "";
    }
    std::cout << time_point << " ";

    size_t event_id{0};
    input >> event_id;
    std::cout << event_id << " ";

    std::string client_name;
    input >> client_name;
    std::cout << client_name << " ";

    size_t table_id{0};
    if (event_id == 2) {
      input >> table_id;
      std::cout << table_id << " ";
    }
    std::cout << "\n";

    ink::Event event{ink::Clock{time_point}, event_id, client_name, table_id};
    computer_club_manager.ProceedEvent(std::move(event));
  }
}
