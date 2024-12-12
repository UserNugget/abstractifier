#include "world/world.h"
#include "world/enemy.h"
#include "util/lib.h"
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

    if (world->entities->size() < ENTITY_CAP) {
      Entity* entity = world->entities->at(0);
      if (entity != nullptr) {
        world->mutex.lock();
        for (int i = 0; i < 6; ++i) {
#define RND_F ((float) rand() / (float) RAND_MAX)
          float w = RND_F * 6.283185307179586; // rand * radians(360)

          float offsetX = cosf(w) * (1056 + (RND_F * 1536.0f));
          float offsetY = sinf(w) * (1056 + (RND_F * 1536.0f));

          world->entities->emplace_back(Enemy::allocate(entity->x + offsetX, entity->y + offsetY, 48 + RND_F * 48, 48 + RND_F * 48));
        }
        world->mutex.unlock();
      }
    }

    for (Entity* entity : *world->entities) {
      entity->oldX = entity->x;
      entity->oldY = entity->y;
      entity->tick(*world);
      entity->ticks++;
    }

    world->mutex.lock();
    std::vector<Entity*>& entities = *world->entities;
    entities.erase(std::remove_if(entities.begin(), entities.end(), [world](Entity* entity) {
      if (entity->removed) {
        entity->remove(*world);
      }

      return entity->removed;
    }), entities.end());

    for (Entity* entity: *world->adding) {
      world->entities->emplace_back(entity);
    }
    world->adding->clear();
    world->mutex.unlock();

    world->tickTime = (int) (timeMillis() - start);
    uint64_t waitDuration = updateRate - world->tickTime;
    if (waitDuration > 0 && waitDuration <= (1000 / TICK_RATE)) {
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


