#include "world/world.h"
#include "world/entity.h"
#include "world/enemy.h"

Enemy::Enemy(float x, float y, float w, float h) : Entity(x, y, w, h) {
  speed = 2;
}

void Enemy::tick(World &world) {
  Entity* client = world.entities->at(0);
  if (client != nullptr) {
    float distance = this->distanceSqaured(*client);
    if (distance < 64 * 64) {
      // TODO: game over screen
      world.gameOver();
      return;
    } else if (distance > 2800 * 2800) {
      world.remove(this);
      return;
    }

    // TODO: better pathfinding
    float clientX = client->x + client->w / 2.0f;
    float clientY = client->y + client->h / 2.0f;
    if (std::abs(x - clientX) < std::abs(y - clientY)) {
      velocityY = clientY > y ? 1 : -1;
    } else {
      velocityX = clientX > x ? 1 : -1;
    }
  }

  Entity::tick(world);
}
