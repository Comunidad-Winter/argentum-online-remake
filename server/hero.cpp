#include "hero.h"

Hero::Hero(int x, int y, int race_id, char repr, int level,
           int strength, int intelligence, int agility, int constitution,
           int base_mana, int f_class_hp, int f_race_hp, int f_race_recovery,
           int f_race_mana, int f_class_mana, int f_class_meditation, int gold, int class_id)
    : BaseCharacter(x, y, race_id, repr, level),
      strength(strength),
      intelligence(intelligence),
      agility(agility),
      constitution(constitution),
      base_mana(base_mana),
      f_class_hp(f_class_hp),
      f_race_hp(f_race_hp),
      f_race_recovery(f_race_recovery),
      f_race_mana(f_race_mana),
      f_class_mana(f_class_mana),
      f_class_meditation(f_class_meditation),
      gold(gold), class_id(class_id) {

        max_hp = constitution * f_class_hp * f_race_hp * level;
      }

Hero::~Hero() {}