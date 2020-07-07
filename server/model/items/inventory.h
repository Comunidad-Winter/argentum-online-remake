#ifndef INVENTORY_H
#define INVENTORY_H
#include "item.h"
#include <vector>
#include "model_exceptions.h"
#include <list>
class Item;
class Inventory {
 public:
  //constructores de equipamiento vacio o con items
  Inventory(std::size_t size);
  Inventory();
  ~Inventory();
  Inventory(const Inventory &) = delete;
  void add_item(Item *item);
  bool has_item(unsigned int id);
  bool is_full();
  bool is_empty();
  //obtiene referencia al item y lo remueve del inventario.
  Item* remove_item(unsigned int id);
  //obtiene una referencia al item, pero no lo remueve del inventario.
  Item* item_with_id(unsigned int id);
  friend class Serializer;
  friend class Drop;
  private:
  std::vector<Item*> items;

};
#endif  // INVENTORY_H
