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
  for (Entity* entity : *world.entities) {
    if (dynamic_cast<ClientEntity*>(entity) != nullptr) continue;
    if (dynamic_cast<Bullet*>(entity) != nullptr) continue;

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

  float currentDeltaX = this->deltaX(deltaTime) - renderer.cameraPosition[0] - (w / 2.0f);
  float currentDeltaY = this->deltaY(deltaTime) - renderer.cameraPosition[1] - (h / 2.0f);
  float width = (float) renderer.game.window->expectedResolution[0];
  float height = (float) renderer.game.window->expectedResolution[1];

  if ((currentDeltaX < 0 && currentDeltaY < 0) || (currentDeltaX > width && currentDeltaY > height)) {
    return;
  }

  shader->show();
  shader->offset(renderer.cameraPosition);
  shader->entityPosition({ currentDeltaX, currentDeltaY });
  shader->entityDimension({ w, h });

  renderer.drawBuffer.pushSquare(currentDeltaX, currentDeltaY, w, h);
  renderer.drawBuffer.draw(renderer.game, *shader);
  shader->hide();
}
