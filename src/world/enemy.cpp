#include "world/world.h"
#include "world/entity.h"
#include "world/enemy.h"
#include "client/entity/client_entity.h"

Enemy::Enemy(float x, float y, float w, float h) : Entity(x, y, w, h) {
  speed = 2;
}

void Enemy::tick(World &world) {
  ClientEntity* client = dynamic_cast<ClientEntity*>(world.at(0));
  if (client != nullptr) {
    if (this->distanceSqaured(*client) < 64 * 64) {
      // TODO: game over screen
      exit(0);
    }

    // TODO: better pathfinding
    float clientX = client->x + client->w / 2.0f;
    if (clientX != x) {
      velocityX = clientX > x ? 1 : -1;
    }

    float clientY = client->y + client->h / 2.0f;
    if (clientY != y) {
      velocityY = clientY > y ? 1 : -1;
    }
  }

  Entity::tick(world);
}
