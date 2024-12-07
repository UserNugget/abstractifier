#ifndef ABSTRACTIFIER_CLIENT_WORLD_H
#define ABSTRACTIFIER_CLIENT_WORLD_H

#include "client/entity/client_entity.h"
#include "world/world.h"

class ClientEntity;

class ClientWorld : public World {
public:
  ClientEntity* entity;

  explicit ClientWorld(Game &game);
};


#endif //ABSTRACTIFIER_CLIENT_WORLD_H
