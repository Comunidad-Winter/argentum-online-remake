#include "monster.h"

#include <iostream>
Monster::Monster(int x, int y, int id, char repr, int hp, int level, int dps,
                 Map *map)
    : BaseCharacter(x, y, id, repr, hp, level, map), dps(dps) {
  std::tuple<int, int> first_move = std::tuple<int, int>(0, 1);
  std::tuple<int, int> second_move = std::tuple<int, int>(1, 0);
  std::tuple<int, int> third_move = std::tuple<int, int>(0, -1);
  std::tuple<int, int> fourth_move = std::tuple<int, int>(-1, 0);
  moves.push_back(first_move);
  moves.push_back(second_move);
  moves.push_back(third_move);
  moves.push_back(fourth_move);
}

void Monster::update() {
  if (!alive) return;
  // Moverse si no hay enemigos cercanos (O si hay muy cerca acercarse a ellos)
  // Atacar si hay enemigo cercano (danando con dps)

  //-------- Movimiento -----------//
  int x_step = std::get<0>(moves.at(current_move));
  int y_step = std::get<1>(moves.at(current_move));
  current_move++;

  int next_x_pos = x_position + x_step;
  int next_y_pos = y_position + y_step;
  if (!map->tile_is_safe(next_x_pos, next_y_pos)) {
    move(next_x_pos, next_y_pos);
  }
  // if (map->can_ocupy_cell(next_x_pos, next_y_pos) &&
  //     !map->tile_is_safe(next_x_pos, next_y_pos)) {
  //   map->ocupy_cell(next_x_pos, next_y_pos);
  //   map->empty_cell(x_position, y_position);
  //   x_position = next_x_pos;
  //   y_position = next_y_pos;
  // }

  if (current_move >= moves.size()) current_move = 0;
}

unsigned int Monster::damage(BaseCharacter *other) {
  if (!alive) throw ModelException("Death monsters can't attack!", "6");
  unsigned int damage = other->receive_damage(dps, false);
  return damage;
}

unsigned int Monster::receive_damage(unsigned int damage, bool critical) {
  if (!alive) throw ModelException("Monster is already death!", "6");
  unsigned int last_hp = current_hp;
  current_hp = std::max(current_hp - damage, (unsigned int)0);
  if (current_hp == 0) alive = false;
  return last_hp - current_hp;
}

bool Monster::is_death() { return alive; }

Monster::~Monster() {}