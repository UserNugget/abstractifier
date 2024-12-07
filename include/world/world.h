#ifndef ABSTRACTIFIER_WORLD_H
#define ABSTRACTIFIER_WORLD_H

#include <vector>
#include <mutex>
#include "game.h"
#include "world/entity.h"
#include "util/lib.h"

class World {
public:
  Game& game;
  std::vector<Entity*>* entities;
  std::vector<Entity*>* removing;
  std::vector<Entity*>* adding;

  std::mutex entitiesMutex;

  int tickRate = TICK_RATE;
  uint64_t tickStart = timeMillis();
  int tickTime = 0;

  int score = 0;

  explicit World(Game &game);

  Entity* at(int id) {
    std::lock_guard<std::mutex> lock(entitiesMutex);
    return entities->at(id);
  }

  static void tick(void* game);

  void add(Entity *entity) const;
  void remove(Entity *entity) const;

  std::vector<Entity*> entitiesSnapshot() {
    entitiesMutex.lock();
    std::vector<Entity*> copied = *entities;
    entitiesMutex.unlock();

    return copied;
  }
};


#endif