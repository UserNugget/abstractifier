#include "world/bullet.h"
#include "world/world.h"
#include "client/draw/shader.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include "util/object_pool.h"

static ObjectPool bulletPool{};

Bullet::Bullet(float x, float y, float speedX, float speedY, float angle) : Entity(x, y, 16, 16), angle(angle), speedX(speedX), speedY(speedY) {
  type = BULLET;
  speed = 1;
}

void Bullet::tick(World &world) {
  velocityX = sinf(angle) * 5.0f * (std::fmax(0.0f, speedX) + 10.0f);
  velocityY = cosf(angle) * 5.0f * (std::fmax(0.0f, speedY) + 10.0f);

  // TODO: rewrite entity storage
  for (Entity* entity : *world.entities) {
    if (entity->type != ENEMY) continue;

    if (this->distanceSqaured(*entity) < 42.0f * 42.0f) {
      world.remove(entity);
      world.remove(this);
      world.addScore(10);
      return;
    }
  }

  Entity::tick(world);

  if (ticks >= 30) {
    world.remove(this);
  }
}

void Bullet::renderTick(Renderer &renderer, float deltaTime) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/bullet.frag");

  float deltaX = this->cameraDeltaX(renderer, deltaTime);
  float deltaY = this->cameraDeltaY(renderer, deltaTime);

  shader->show();
  shader->offset(renderer.cameraPosition);

  renderer.drawBuffer.pushSquare(deltaX, deltaY, w, h);
  renderer.drawBuffer.draw(renderer.game, *shader);
}

Bullet* Bullet::allocate(float x, float y, float speedX, float speedY, float angle) {
  Bullet* bullet = (Bullet*) bulletPool.poll();
  if (bullet) {
    bullet->reset();
    bullet->position(x, y);
    bullet->speedX = speedX;
    bullet->speedY = speedY;
    bullet->angle = angle;
    return bullet;
  }

  return new Bullet(x, y, speedX, speedY, angle);
}

void Bullet::remove(World &world) {
  bulletPool.push(this);
}
