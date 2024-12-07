#include <cmath>
#include "world/entity.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"

Entity::Entity(float x, float y, float w, float h) : x(x), y(y), w(w), h(h), oldX(x), oldY(y) {

}

Entity::~Entity() = default;

void Entity::tick(World& world) {
  velocityX *= 0.95f;
  velocityY *= 0.95f;

  if (std::fabs(velocityX) > 0.001f) {
    x += velocityX * speed;
  }

  if (std::fabs(velocityY) > 0.001f) {
    y += velocityY * speed;
  }
}

void Entity::renderTick(Renderer& renderer, float deltaTime) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/block.frag");

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

float Entity::deltaX(float delta) const {
  return oldX + (std::fabs(x - oldX) <= 0.0001 ? 0 : (x - oldX) * std::fmax(0.0f, std::fmin(delta, 1.0f)));
}

float Entity::deltaY(float delta) const {
  return oldY + (std::fabs(y - oldY) <= 0.0001 ? 0 : (y - oldY) * std::fmax(0.0f, std::fmin(delta, 1.0f)));
}

