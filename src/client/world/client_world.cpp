#include "client/world/client_world.h"

ClientWorld::ClientWorld(Game &game) : World(game), entity(new ClientEntity(1, 1, 64, 64, game.input)) {
  entities->emplace_back(entity);
}