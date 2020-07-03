#include "entity.h"

Entity::Entity(unsigned int unique_id, int x, int y, int type, char repr)
    : unique_id(unique_id),
      x_position(x),
      y_position(y),
      type(type),
      alive(true),
      orientation(orientation_down),
      representation(repr) {}

Entity::~Entity() {}

char Entity::char_representation() { return representation; }
