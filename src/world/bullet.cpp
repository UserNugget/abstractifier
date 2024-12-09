#include "world/bullet.h"
#include "world/world.h"
#include "client/draw/shader.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include <cmath>

Bullet::Bullet(float x, float y, float speedX, float speedY, float angle) : Entity(x, y, 16, 16), angle(angle), speedX(speedX), speedY(speedY) {
  speed = 1;
}

void Bullet::tick(World &world) {
  velocityX = sinf(angle) * 5.0f * (std::fmax(0.0f, speedX) + 10.0f);
  velocityY = cosf(angle) * 5.0f * (std::fmax(0.0f, speedY) + 10.0f);

  // TODO: rewrite entity storage
  Entity* player = world.entities->at(0);
  for (Entity* entity : *world.entities) {
    if (entity == player || entity == this || dynamic_cast<Bullet*>(entity) != nullptr) continue;

    if (this->distanceSqaured(*entity) < 42.0f * 42.0f) {
      world.remove(entity);
      world.remove(this);
      world.score += 10;
      return;
    }
  }

  Entity::tick(world);

  if (ticks >= 40) {
    world.remove(this);
  }
}

void Bullet::renderTick(Renderer &renderer, float deltaTime) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/bullet.frag");

  float deltaX = this->cameraDeltaX(renderer, deltaTime);
  float deltaY = this->cameraDeltaY(renderer, deltaTime);

  shader->show();
  shader->offset(renderer.cameraPosition);
  shader->entityPosition({ deltaX, deltaY });
  shader->entityDimension({ w, h });

  renderer.drawBuffer.pushSquare(deltaX, deltaY, w, h);
  renderer.drawBuffer.draw(renderer.game, *shader);
  shader->hide();
}
