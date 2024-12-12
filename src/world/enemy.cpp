#include "math/vectors.h"
#include "world/world.h"
#include "world/entity.h"
#include "world/enemy.h"
#include "util/object_pool.h"

static ObjectPool enemyPool;

Enemy::Enemy(float x, float y, float w, float h) : Entity(x, y, w, h) {
  type = ENEMY;
  speed = 2;
}

void Enemy::tick(World &world) {
  Entity* client = world.entities->at(0);
  if (client != nullptr) {
    float distance = this->distanceSqaured(*client);
    if (distance < std::pow(client->w / 2.0f, 2)) {
      // TODO: game over screen
      world.gameOver();
      return;
    } else if (distance > 2800 * 2800) {
      world.remove(this);
      return;
    }

    float rotation = angle({ x, y }, { client->x, client->y });
    velocityX = std::sinf(rotation);
    velocityY = std::cosf(rotation);
  }

  Entity::tick(world);
}

Enemy* Enemy::allocate(float x, float y, float w, float h) {
  Enemy* enemy = (Enemy*) enemyPool.poll();
  if (enemy) {
    enemy->reset();
    enemy->position(x, y);
    enemy->w = w;
    enemy->h = h;
    return enemy;
  }

  return new Enemy(x, y, w, h);
}

void Enemy::remove(World &world) {
  enemyPool.push(this);
}
