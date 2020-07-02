#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "entity.h"
#include "model_exceptions.h"
#include "map.h"
class Projectile : public Entity {
 public:
  Projectile(unsigned int unique_id, int x, int y, int type, char repr, const unsigned int damage,
             const bool critical, const unsigned int attacker_id,
             const unsigned int range, const orientation_t orientation, Map *map);
  virtual ~Projectile();
  Projectile(const Projectile &) = delete;
  // Mover proyectil, chequear colision
  void update() override;
  bool collided;

 private:
  unsigned int damage;
  bool critical;
  // para notificar, actualizar xp, etc
  unsigned int attacker_id;
  unsigned int range;
  orientation_t orientation;
  Map *map;
  int collided_player;
  int get_next_x();
  int get_next_y();
  
};

#endif  // PROJECTILE_H
