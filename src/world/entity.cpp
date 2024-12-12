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
  float deltaX = this->cameraDeltaX(renderer, deltaTime);
  float deltaY = this->cameraDeltaY(renderer, deltaTime);
  renderer.entityBuffer.pushSquare(deltaX, deltaY, w, h);
}

float Entity::deltaX(float delta) const {
  return oldX + (x - oldX) * delta;
}

float Entity::deltaY(float delta) const {
  return oldY + (y - oldY) * delta;
}

bool Entity::outOfBounds(Renderer &renderer, float deltaTime) const {
  return std::abs(this->cameraDeltaX(renderer, deltaTime) - renderer.game.window->scaleX) > renderer.game.window->scaleX + w
      || std::abs(this->cameraDeltaY(renderer, deltaTime) - renderer.game.window->scaleY) > renderer.game.window->scaleY + h;
}

float Entity::cameraDeltaX(Renderer& renderer, float deltaTime) const {
  return this->deltaX(deltaTime) - renderer.cameraPosition[0] - (w / 2.0f);
}

float Entity::cameraDeltaY(Renderer& renderer, float deltaTime) const {
  return this->deltaY(deltaTime) - renderer.cameraPosition[1] - (h / 2.0f);
}

void Entity::draw(Renderer &renderer) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/block.frag");
  if (renderer.entityBuffer.empty()) {
    return;
  }

  shader->show();
  shader->offset(renderer.cameraPosition);
  renderer.entityBuffer.draw(renderer.game, *shader);
  shader->hide();
}

