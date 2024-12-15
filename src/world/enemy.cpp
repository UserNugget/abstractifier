#include "client/draw/renderer.h"
#include "util/object_pool.h"
#include "world/world.h"
#include "world/entity.h"
#include "world/enemy.h"
#include "math/vectors.h"

static ObjectPool enemyPool;

Enemy::Enemy(float x, float y, float w, float h) : Entity(x, y, w, h) {
  type = ENEMY;
  speed = 2;
}

void Enemy::tick(World &world) {
  Entity* client = world.entities->at(0);
  if (client != nullptr) {
    if (client->intersect(*this)) {
      // TODO: game over screen
      world.gameOver();
      return;
    } else if (this->distanceSquared(*client) > 2800 * 2800) {
      world.remove(this);
      return;
    }

    float rotation = vec2f { client->x, client->y }.angle({ x, y });
    velocityX = sinf(rotation);
    velocityY = cosf(rotation);
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

void Enemy::renderTick(Renderer &renderer, float deltaTime) {
  float deltaX = this->cameraDeltaX(renderer, deltaTime);
  float deltaY = this->cameraDeltaY(renderer, deltaTime);
  renderer.entityBuffer.pushSquare(deltaX, deltaY, w, h, LIB_RGB(250, 15, 15));
}
