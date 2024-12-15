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

  Entity::tick(world);

  // TODO: rewrite entity storage
  for (Entity* entity : *world.entities) {
    if (entity->type != ENEMY || entity->removed) continue;

    vec2f entityPos = { entity->x - entity->w / 2.0f, entity->y - entity->h / 2.0f };
    vec2f entityBox = { entity->x + entity->w / 2.0f, entity->y + entity->h / 2.0f };

    if (rayIntersect(entityPos, entityBox, { oldX, oldY }, { x, y })) {
      world.remove(entity);
      world.remove(this);
      world.addScore(10);
      return;
    }
  }

  if (ticks >= 30) {
    world.remove(this);
  }
}

void Bullet::renderTick(Renderer &renderer, float deltaTime) {
  renderer.bulletBuffer.pushSquare(this->cameraDeltaX(renderer, deltaTime), this->cameraDeltaY(renderer, deltaTime), w, h);
}

void Bullet::draw(Renderer &renderer) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/entity/bullet.frag");
  if (renderer.bulletBuffer.empty()) {
    return;
  }

  shader->show();
  shader->offset(renderer.cameraPosition);
  renderer.bulletBuffer.draw(*shader);
  shader->hide();
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
