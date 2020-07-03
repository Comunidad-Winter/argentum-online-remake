#include "monsters_manager.h"

MonstersManager::MonstersManager() {
  last_update_time = std::chrono::high_resolution_clock::now();
}

MonstersManager::~MonstersManager() {}

void MonstersManager::update(std::map<unsigned int, Monster *> &monsters) {
  auto actual_time = std::chrono::high_resolution_clock::now();
  auto time_difference = actual_time - last_update_time;

  for (auto &monster : monsters) {
    if (time_difference.count() >= 1000000000) {
      monster.second->auto_move();
      last_update_time = actual_time;
    }
    monster.second->clear_effects();
  }
}

void MonstersManager::remove_death_monsters(
    std::map<unsigned int, Monster *> &monsters, Map *map) {
  for (auto it = monsters.cbegin(); it != monsters.cend();) {
    if (it->second->alive == false) {
      int x_pos = it->second->x_position;
      int y_pos = it->second->y_position;
      map->empty_cell(x_pos, y_pos);
      delete it->second;
      it = monsters.erase(it++);
    } else {
      ++it;
    }
  }
}
