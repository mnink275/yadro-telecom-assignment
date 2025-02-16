#pragma once

#include <cstddef>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <clock.hpp>

namespace ink {

struct Table final {
  bool is_occupied{false};
  Clock occupied_time{};
  size_t revenue{0};
};

struct Event final {
  Clock time_point{};
  size_t event_id{0};
  std::string client_name;
  size_t table_id{0};
};

struct ClientInfo final {
  std::optional<size_t> occupied_table_id{std::nullopt};
  Clock occupied_table_from{};
};

class ComputerClub final {
 public:
  ComputerClub(size_t num_tables, Clock open_time, Clock close_time,
               size_t per_hour_price);

  ComputerClub(const ComputerClub&) = delete;
  ComputerClub& operator=(const ComputerClub&) = delete;

  ComputerClub(ComputerClub&&) = delete;
  ComputerClub& operator=(ComputerClub&&) = delete;

  ~ComputerClub();

  void ProceedEvent(Event&& event);

  void HandleEnterClub(Event&& event);
  void HandleOccupyTable(Event&& event);
  void HandleWaitFreeTable(Event&& event);
  void HandleLeaveClub(Event&& event);

 private:
  static void GenerateClienLeaveEvent(Clock time, std::string_view client_name);
  static void GenerateClienOccupyTableEvent(Clock time, std::string_view client_name, size_t table_id);
  static void GenerateErrorEvent(Clock time, std::string_view error);

  std::pair<Clock, size_t> GetTableOccupiedTimeAndRevenue(const std::string& client_name, Clock time_point);

 private:
  size_t num_tables_;
  Clock open_time_;
  Clock close_time_;
  size_t per_hour_price_;

  std::unordered_map<std::string, ClientInfo> in_club_clients_info_;

  std::vector<Table> tables_;
  size_t num_free_tables_;

  std::queue<std::string> waiting_group_;
};

}  // namespace ink
