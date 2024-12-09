#include "world/world.h"
#include "util/lib.h"
#include "world/enemy.h"
#include "client/entity/client_entity.h"
#include <algorithm>
#include <cmath>
#include <mutex>

World::World(Game &game) : entities(new std::vector<Entity*>()), adding(new std::vector<Entity*>()), game(game) {

}

#ifdef _WIN32
#include <windows.h>

static NTSTATUS(__stdcall *NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtDelayExecution");
static NTSTATUS(__stdcall *ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwSetTimerResolution");
#endif

void World::tick(void* param) {
  LOG("ticking thread started")

  World* world = (World*) param;
#ifdef __WINDOWS__
  static bool once = true;
  if (once) {
      ULONG actualResolution;
      ZwSetTimerResolution(1, true, &actualResolution);
      once = false;
  }
#endif

  uint64_t updateRate = 1000 / world->tickRate;
  while (!world->game.quit) {
    uint64_t start = timeMillis();
    world->tickStart = start;

    if (world->entities->size() < 250) {
      ClientEntity* entity = dynamic_cast<ClientEntity*>(world->entities->at(0));
      if (entity != nullptr) {
        float w = (float) rand() / (float) RAND_MAX;

        world->mutex.lock();
        world->entities->emplace_back(new Enemy(entity->x + (cosf(w) - 1.0f) * 2048, entity->y + (sinf(w) - 1.0f) * 2048, 48, 48));
        world->mutex.unlock();
      }
    }

    for (Entity* entity : world->entitiesSnapshot()) {
      entity->oldX = entity->x;
      entity->oldY = entity->y;
      entity->tick(*world);
      entity->ticks++;
    }

    world->mutex.lock();
    std::vector<Entity*>& entities = *world->entities;
    entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity* entity) {
      return entity->removed;
    }), entities.end());

    for (Entity* entity: *world->adding) {
      world->entities->emplace_back(entity);
    }
    world->adding->clear();
    world->mutex.unlock();

    world->tickTime = (int) (timeMillis() - start);
    uint64_t waitDuration = updateRate - world->tickTime;
    if (waitDuration > 0) {
#ifdef __WINDOWS__
      LARGE_INTEGER interval;
      interval.QuadPart = -1 * (int) ((int) waitDuration * 10000);
      NtDelayExecution(false, &interval);
#else
      WAIT(waitDuration);
#endif
    }
  }
}

void World::remove(Entity *entity) const {
  entity->removed = true;
}

void World::add(Entity *entity) const {
  adding->emplace_back(entity);
}

void World::gameOver() {
  Entity* player = this->entities->at(0);
  for (Entity* entity: *entities) {
    entity->removed = true;
  }

  player->removed = false;
  player->x = 0;
  player->y = 0;
  score = 0;
}


