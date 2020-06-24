
#ifndef HERO_H
#define HERO_H

#include <cmath>

#include "base_character.h"
#include "equipment.h"
#include "inventory.h"
#include "monster.h"
#include "stdint.h"
// meter en el json!
#define CRITICAL_DAMAGE_MULTIPLIER 2
#define INVENTORY_SIZE 20
class Inventory;
class Item;
class Hero : public BaseCharacter {
 public:
  // constructor para heroes nuevos
  Hero(int x, int y, int race_id, char repr, int level, int strength,
       int intelligence, int agility, int constitution, int f_class_hp,
       int f_race_hp, int f_race_recovery, int f_race_mana, int f_class_mana,
       int f_class_meditation, int gold, int class_id, Map *map);
  // devuelve el dano causado
  void update() override;
  unsigned int damage(Hero *other);
  unsigned int damage(Monster *m);
  // devuelve el dano que efectivamente recibi
  virtual unsigned int receive_damage(unsigned int damage, bool critical) override;
  // PRE: Se llama a unequip_x antes de equip_x para que lo guarde en
  // inventario.
  void equip_weapon(unsigned int weapon_id);
  void equip_staff(unsigned int staff_id);
  void equip_shield(unsigned int shield_id);
  void equip_helmet(unsigned int helmet_id);
  void equip_armour(unsigned int armour_id);
  void unequip_weapon();
  void unequip_staff();
  void unequip_shield();
  void unequip_helmet();
  void unequip_armour();
  // devuelve el Item en el inventario con el id buscado. Devuelve nullptr si no
  // lo tiene
  Item *remove_item(unsigned int item_id);
  // Agrega un item al inventario, lanza excepcion si este esta lleno
  void add_item(Item *item);
  virtual ~Hero();
  Hero(const Hero &) = delete;
  friend class Staff;
  int current_mana;
  int max_mana;

 private:
  int strength;
  int intelligence;
  int agility;
  int constitution;
  int f_class_hp;
  int f_race_hp;
  int f_race_recovery;
  int f_race_mana;
  int f_class_mana;
  int f_class_meditation;
  int gold;
  int class_id;
  int alive = true;
  Equipment *equipment;
  Inventory *inventory;
  unsigned int calculate_damage();
  bool close_enough(BaseCharacter *other);
};
#endif  // HERO_H
