#include <computer_club.hpp>

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cassert>

namespace ink {

ComputerClub::ComputerClub(size_t num_tables, Clock open_time,
                           Clock close_time, size_t per_hour_price)
    : num_tables_(num_tables),
      open_time_(open_time),
      close_time_(close_time),
      per_hour_price_(per_hour_price),
      in_club_clients_info_(),
      tables_(num_tables),
      num_free_tables_(num_tables),
      waiting_group_() {
  std::cout << open_time_.ToString() << '\n';
}

ComputerClub::~ComputerClub() try {
  std::vector<std::string_view> clients_to_leave;
  clients_to_leave.reserve(in_club_clients_info_.size());
  for (auto& [client_name, client_info] : in_club_clients_info_) {
    clients_to_leave.push_back(client_name);
    if (client_info.occupied_table_id.has_value()) {
      const auto occupied_table_id = client_info.occupied_table_id.value();
      auto [occupied_time, revenue] = GetTableOccupiedTimeAndRevenue(client_name, close_time_);

      tables_[occupied_table_id - 1].occupied_time += occupied_time;
      tables_[occupied_table_id - 1].revenue += revenue;
    }
  }

  std::ranges::sort(clients_to_leave);
  for (const auto& client_name : clients_to_leave) {
    
    GenerateClienLeaveEvent(close_time_, client_name);
  }

  std::cout << close_time_.ToString() << '\n';

  for (size_t i = 0; i < num_tables_; ++i) {
    std::cout << i + 1 << ' ' << tables_[i].revenue << ' ' << tables_[i].occupied_time.ToString() << '\n';
  }
} catch (const std::exception& ex) {
  std::cerr << ex.what() << '\n';
  std::cerr << "Expection has been thrown inside ~ComputerClub";
}

void ComputerClub::ProceedEvent(Event&& event) {
  switch (event.event_id) {
    case 1:
      HandleEnterClub(std::move(event));
      break;
    case 2:
      HandleOccupyTable(std::move(event));
      break;
    case 3:
      HandleWaitFreeTable(std::move(event));
      break;
    case 4:
      HandleLeaveClub(std::move(event));
      break;
    default:
      throw std::runtime_error{"Unknown EventID"};
  }
}

void ComputerClub::HandleEnterClub(Event&& event) {
  if (in_club_clients_info_.contains(event.client_name)) {
    GenerateErrorEvent(event.time_point, "YouShallNotPass");
    return;
  }
  if (event.time_point < open_time_ || close_time_ < event.time_point) {
    GenerateErrorEvent(event.time_point, "NotOpenYet");
    return;
  }

  in_club_clients_info_.emplace(event.client_name, ClientInfo{std::nullopt});
}

void ComputerClub::HandleOccupyTable(Event&& event) {
  if (!in_club_clients_info_.contains(event.client_name)) {
    GenerateErrorEvent(event.time_point, "ClientUnknown");
    return;
  }
  if (event.table_id == 0 || event.table_id > num_tables_) {
    GenerateErrorEvent(event.time_point, "IncorrectTableId");
    return;
  }
  if (tables_[event.table_id - 1].is_occupied) {
    GenerateErrorEvent(event.time_point, "PlaceIsBusy");
    return;
  }

  in_club_clients_info_[event.client_name].occupied_table_id = event.table_id;
  in_club_clients_info_[event.client_name].occupied_table_from = event.time_point;
  tables_[event.table_id - 1].is_occupied = true;
  --num_free_tables_;
}

void ComputerClub::HandleWaitFreeTable(Event&& event) {
  if (num_free_tables_ > 0) {
    GenerateErrorEvent(event.time_point, "ICanWaitNoLonger!");
    return;
  }
  if (!in_club_clients_info_.contains(event.client_name)) {
    GenerateErrorEvent(event.time_point, "ClientUnknown");
    return;
  }
  if (waiting_group_.size() == num_tables_) {
    GenerateClienLeaveEvent(event.time_point, event.client_name);
    return;
  }

  waiting_group_.push(event.client_name);
}

void ComputerClub::HandleLeaveClub(Event&& event) {
  if (!in_club_clients_info_.contains(event.client_name)) {
    GenerateErrorEvent(event.time_point, "ClientUnknown");
    return;
  }

  const auto occupied_table_id = in_club_clients_info_[event.client_name].occupied_table_id;
  if (!occupied_table_id.has_value()) {
    return;
  }

  auto [occupied_time, revenue] = GetTableOccupiedTimeAndRevenue(event.client_name, event.time_point);
  tables_[*occupied_table_id - 1].occupied_time += occupied_time;
  tables_[*occupied_table_id - 1].revenue += revenue;

  in_club_clients_info_.erase(event.client_name);

  if (waiting_group_.empty()) {
    tables_[*occupied_table_id - 1].is_occupied = false;
    ++num_free_tables_;
    return;
  }

  auto client_name = std::move(waiting_group_.front());
  waiting_group_.pop();

  in_club_clients_info_[client_name].occupied_table_id = occupied_table_id;
  in_club_clients_info_[client_name].occupied_table_from = event.time_point;
  GenerateClienOccupyTableEvent(event.time_point, client_name, occupied_table_id.value());
}

void ComputerClub::GenerateClienLeaveEvent(Clock time, std::string_view client_name) {
  static constexpr auto EventID = 11;
  std::cout << time.ToString()  << " " << EventID << " " << client_name << "\n";
}

void ComputerClub::GenerateClienOccupyTableEvent(Clock time, std::string_view client_name, size_t table_id) {
  static constexpr auto EventID = 12;
  std::cout << time.ToString()  << " " << EventID << " " << client_name << " " << table_id << "\n";
}

void ComputerClub::GenerateErrorEvent(Clock time, std::string_view error) {
  static constexpr auto EventID = 13;
  std::cout << time.ToString()  << " " << EventID << " " << error << "\n";
}

std::pair<Clock, size_t> ComputerClub::GetTableOccupiedTimeAndRevenue(const std::string& client_name, Clock time_point) {
  assert(in_club_clients_info_.contains(client_name));
  assert(in_club_clients_info_[client_name].occupied_table_id.has_value());

  const auto& client_info = in_club_clients_info_[client_name];
  const auto occupied_by_client_time = time_point - client_info.occupied_table_from;

  const auto [hours, minutes] = occupied_by_client_time.GetTime();
  const auto payed_hours = hours + (minutes > 0);

  return {occupied_by_client_time, payed_hours * per_hour_price_};
}

}  // namespace ink
