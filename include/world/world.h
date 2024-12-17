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
  std::vector<Entity*>* adding;

  std::mutex mutex;

  int tickRate = TICK_RATE;
  uint64_t tickStart = timeMillis();
  int tickTime = 0;
  int tickEnd = 0;

  int abilityScore = 0;
  int score = 0;

  bool paused = false;

  explicit World(Game &game);

  virtual ~World();

  Entity* at(int id) {
    std::lock_guard<std::mutex> lock(mutex);
    return entities->at(id);
  }

  static void tick(void* game);

  void add(Entity *entity) const;
  void remove(Entity *entity) const;

  void addScore(int score) {
    this->score += score;
    this->abilityScore += score;
  }

  std::vector<Entity*> entitiesSnapshot() {
    std::lock_guard<std::mutex> lock(mutex);
    return *entities;
  }

  void gameOver();
};


#endif