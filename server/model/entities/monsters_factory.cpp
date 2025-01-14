#include "monsters_factory.h"

Monster* MonstersFactory::create_random_monster(Json::Value& monsters_cfg,
                                                unsigned int& entities_ids,
                                                Map* map) {
  int random_int =
      HelperFunctions::random_int(first_monster_id, last_monster_id);
  Json::Value monster_cfg = Json::arrayValue;
  std::string monster_name = "";
  switch (random_int) {
    case zombie:
      monster_cfg = monsters_cfg["zombie"];
      monster_name = "Zombie";
      break;

    case spider:
      monster_cfg = monsters_cfg["spider"];
      monster_name = "Ara";
      monster_name.push_back((char)ENIE);
      monster_name += "a";
      break;

    case goblin:
      monster_cfg = monsters_cfg["goblin"];
      monster_name = "Goblin";
      break;

    case skeleton:
      monster_cfg = monsters_cfg["skeleton"];
      monster_name = "Esqueleto";
      break;
  }

  std::tuple<int, int> coordinates = map->get_random_free_unsafe_space();
  int x = std::get<0>(coordinates);
  int y = std::get<1>(coordinates);
  Monster* m =
      new Monster(entities_ids, x, y, monster_cfg["id"].asInt(), 'g',
                  monster_cfg["maxHp"].asInt(), monster_cfg["level"].asInt(),
                  monster_cfg["dps"].asInt(), map, monster_name,
                  monster_cfg["criticalDamageMultiplier"].asFloat(), 
                  monster_cfg["autoAttackDistance"].asInt(),
                  monster_cfg["autoFollowDistance"].asInt() );
  map->ocupy_cell(x, y, entities_ids);
  return m;
}