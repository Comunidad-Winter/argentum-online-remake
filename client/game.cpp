#include "game.h"

Game::Game() {}

// El viewport es la "ventana" por la cual vemos el juego
Game::Game(int id_player, int scr_width, int scr_height, float w_ratio,
           float h_ratio, map_t new_map)
    : id_hero(id_player),
      screen_width(scr_width - WIDTH_UI * w_ratio),
      screen_height(scr_height - HEIGHT_UI * h_ratio),
      width_ratio(w_ratio),
      height_ratio(h_ratio) {
  change_map(new_map);
  map_piece = {0, 0, screen_width, screen_height};
  viewport = {0, (int)(HEIGHT_UI * h_ratio), screen_width, screen_height};
}

Game::~Game() {
  if (!characters.empty()) {
    clean_all_characters(false);
  }
}

Game::Game(const Game& other_game) {
  id_hero = other_game.id_hero;
  screen_width = other_game.screen_width;
  screen_height = other_game.screen_height;
  width_ratio = other_game.width_ratio;
  height_ratio = other_game.height_ratio;
  background = other_game.background;
  static_objects = other_game.static_objects;
  map_piece = other_game.map_piece;
  viewport = other_game.viewport;

  if (!characters.empty()) {
    clean_all_characters(false);
  }

  std::map<int, Character*>::const_iterator it;
  for (it = other_game.characters.begin(); it != other_game.characters.end();
       ++it) {
    Npc* npc = dynamic_cast<Npc*>(it->second);
    PlayableCharacter* pc = dynamic_cast<PlayableCharacter*>(it->second);
    if (npc != nullptr)
      characters[it->first] = new Npc(*npc);
    else
      characters[it->first] = new PlayableCharacter(*pc);
  }
  items = other_game.items;
}

Game& Game::operator=(const Game& other_game) {
  id_hero = other_game.id_hero;
  screen_width = other_game.screen_width;
  screen_height = other_game.screen_height;
  width_ratio = other_game.width_ratio;
  height_ratio = other_game.height_ratio;
  background = other_game.background;
  static_objects = other_game.static_objects;
  map_piece = other_game.map_piece;
  viewport = other_game.viewport;

  if (!characters.empty()) {
    characters.clear();
  }

  std::map<int, Character*>::const_iterator it;
  for (it = other_game.characters.begin(); it != other_game.characters.end();
       ++it) {
    Npc* npc = dynamic_cast<Npc*>(it->second);
    PlayableCharacter* pc = dynamic_cast<PlayableCharacter*>(it->second);
    if (npc != nullptr)
      characters[it->first] = new Npc(*npc);
    else
      characters[it->first] = new PlayableCharacter(*pc);
  }
  items = other_game.items;
  return *this;
}

void Game::update_character(int id, entity_t entity_type, int new_x, int new_y,
                            move_t orientation, bool ghost, bool meditating,
                            id_texture_t helmet, id_texture_t armor,
                            id_texture_t shield, id_texture_t weapon,
                            std::vector<sound_t>& incoming_sounds) {
  // Necesitamos traducir las posiciones de tiles a pixeles
  int x_render_scale = new_x * TILE_SIZE;
  int y_render_scale = new_y * TILE_SIZE;

  // Chequeamos si hubo un cambio de posicion
  if (characters[id]->change_position(x_render_scale, y_render_scale,
                                      orientation)) {
    // Si se actualiza el heroe la camara lo tiene que seguir
    if (id == id_hero)
      update_map(x_render_scale - characters[id]->get_x(),
                 y_render_scale - characters[id]->get_y());

    // Si el personaje se mueve dentro del viewport emite sonido
    if ((map_piece.x < x_render_scale) &&
        (x_render_scale < map_piece.x + screen_width))
      if ((map_piece.y < y_render_scale) &&
          (y_render_scale < map_piece.y + screen_height))
        incoming_sounds.push_back(characters[id]->sound_walk());

    // Actualizamos la posición
    characters[id]->update_position(new_x, new_y, orientation);
  }
  // En caso de que sea un personaje jugable actualizamos su equipamiento
  if (entity_type == HUMAN || entity_type == ELF || entity_type == GNOME ||
      entity_type == DWARF)
    dynamic_cast<PlayableCharacter*>(characters[id])
        ->update_equipment(ghost, meditating, helmet, armor, shield, weapon,
                           incoming_sounds);
}

void Game::update_map(int new_x, int new_y) {
  map_piece.x += new_x;
  map_piece.y += new_y;

  // Como renderizar cuando nos aceramos al borde derecho
  if (map_piece.x > MAP_SIZE - screen_width) {
    viewport.w = screen_width - (map_piece.x - (MAP_SIZE - screen_width));
    map_piece.w = screen_width - (map_piece.x - (MAP_SIZE - screen_width));
  } else {
    viewport.w = screen_width;
    map_piece.w = screen_width;
  }
  // Como renderizar cuando nos acercamos al borde inferior
  if (map_piece.y > MAP_SIZE - screen_height) {
    viewport.h = screen_height - (map_piece.y - (MAP_SIZE - screen_height));
    map_piece.h = screen_height - (map_piece.y - (MAP_SIZE - screen_height));
  } else {
    viewport.h = screen_height;
    map_piece.h = screen_height;
  }
  // Como renderizar cuando nos aceramos al borde izquierdo
  // No entiendo como, pero la parte de map_piece.w funciona.
  if (map_piece.x < 0) {
    viewport.x = -map_piece.x;
    viewport.w = screen_width + map_piece.x;
    map_piece.w = screen_width;
  } else {
    viewport.x = 0;
    map_piece.w = screen_width;
  }
  // Como renderizar cuando nos aceramos al borde superior
  if (map_piece.y < 0) {
    viewport.y = -map_piece.y + HEIGHT_UI * height_ratio;
    map_piece.h = screen_height - map_piece.y;
  } else {
    viewport.y = HEIGHT_UI * height_ratio;
    map_piece.h = screen_height;
  }
}

void Game::update_spellbound(int id, id_texture_t spell_type, int lifetime) {
  characters[id]->set_spell(spell_type, lifetime);
}

void Game::render(SDL_Renderer* renderer) {
  // Renderizamos el fondo estático
  texture_manager.get_texture(background)
      .render(renderer, &map_piece, &viewport);

  // Renderizamos las entidades
  render_entities(renderer);

  // Renderizamos los objetos que tapan a las entidades
  texture_manager.get_texture(static_objects)
      .render(renderer, &map_piece, &viewport);
}

void Game::load_character(int id, entity_t entity_type, int x, int y,
                          move_t orientation, bool alive, id_texture_t helmet,
                          id_texture_t armor, id_texture_t shield,
                          id_texture_t weapon) {
  int x_render_scale = x * TILE_SIZE;
  int y_render_scale = y * TILE_SIZE;
  if (entity_type == HUMAN || entity_type == ELF || entity_type == GNOME ||
      entity_type == DWARF) {
    characters[id] = new PlayableCharacter(entity_type, x_render_scale,
                                           y_render_scale, orientation, alive,
                                           helmet, armor, shield, weapon);
    // Si cargamos a hero por primera vez ubicamos la parte del mapa que
    // queremos ver
    if (id == id_hero)
      update_map(x_render_scale - screen_width / 2,
                 y_render_scale - screen_height / 2);

  } else {
    characters[id] =
        new Npc(entity_type, x_render_scale, y_render_scale, orientation);
  }
}

void Game::load_item(int id, id_texture_t item, int new_x, int new_y) {
  items[id].type_item = item;
  items[id].x = new_x * TILE_SIZE;
  items[id].y = new_y * TILE_SIZE;
}

void Game::render_entities(SDL_Renderer* renderer) {
  std::map<int, Character*>::iterator it;
  std::map<int, dropped_t>::iterator it2;

  for (it2 = items.begin(); it2 != items.end(); ++it2) {
    // El item DEBE estar dentro del viewport
    if ((map_piece.x <= it2->second.x) &&
        (it2->second.x <= map_piece.x + screen_width))
      if ((map_piece.y <= it2->second.y) &&
          (it2->second.y <= map_piece.y + screen_height)) {
        texture_manager.get_texture(it2->second.type_item)
            .render(renderer, NULL, it2->second.x - map_piece.x,
                    it2->second.y - map_piece.y + HEIGHT_UI * height_ratio);
      }
  }

  for (it = characters.begin(); it != characters.end(); ++it) {
    // El personaje DEBE estar dentro del viewport
    if ((map_piece.x <= it->second->get_x()) &&
        (it->second->get_x() <= map_piece.x + screen_width))
      if ((map_piece.y <= it->second->get_y()) &&
          (it->second->get_y() <= map_piece.y + screen_height)) {
        it->second->render(renderer, map_piece.x,
                           map_piece.y - HEIGHT_UI * height_ratio);
      }
  }
}

void Game::change_map(map_t new_map) {
  switch (new_map) {
    case GRASS_MAP:
      background = ID_MAP_GRASS_BACKGROUND;
      static_objects = ID_MAP_GRASS_OBJECTS;
      break;

    case DESERT_MAP:
      background = ID_MAP_DESERT_BACKGROUND;
      static_objects = ID_MAP_DESERT_OBJECTS;
      break;

    case ARGAL_MAP:
      background = ID_MAP_ARGAL_BACKGROUND;
      static_objects = ID_MAP_ARGAL_OBJECTS;
      break;
    case ICE_MAP:
      background = ID_MAP_ICE_BACKGROUND;
      static_objects = ID_MAP_ICE_OBJECTS;
      break;

    default:
      break;
  }
}

void Game::clean_entity(int i, entity_t type_entity) {
  if (type_entity == ITEM) {
    items.erase(i);
  } else {
    delete characters[i];
    characters.erase(i);
  }
}

void Game::clean_all_characters(bool also_hero) {
  std::map<int, Character*>::iterator it;
  for (it = characters.begin(); it != characters.end();) {
    if (it->first != id_hero || also_hero) {
      delete it->second;
      it = characters.erase(it);
    } else {
      ++it;
    }
  }
  items.clear();
}
