#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

#include "../util/json/json-forwards.h"
#include "../util/json/json.h"
#include "commands_blocking_queue.h"
#include "commands_sender.h"
#include "common_socket.h"
#include "event_handler.h"
#include "events_queue.h"
#include "game.h"
#include "game_renderer.h"
#include "game_updater.h"
#include "protocol.h"
#include "texture.h"

class Client {
 private:
  // Por ahora lo dejamos harcodeado
  int player_id;
  Socket socket;
  bool is_running;

 public:
  Client(const char* host, const char* port);
  ~Client();
  void play();
};

#endif
