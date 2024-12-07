#include "client/entity/client_entity.h"
#include "client/input/input.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include "world/bullet.h"

ClientEntity::ClientEntity(float x, float y, float w, float h, Input* input) : Entity(x, y, w, h), input(input) {

}

void ClientEntity::tick(World& world) {
  Entity::tick(world);

  float speedX = 0;
  float speedY = 0;

  if (input->forward) speedY++;
  if (input->right) speedX++;
  if (input->backward) speedY--;
  if (input->left) speedX--;

  velocityX = speedX;
  velocityY = speedY;
  speed = input->scale;

  if (input->shoot && (ticks % 2) == 0) {
    vec2f& mouse = world.game.window->mouseWorld;
    world.add(new Bullet(x + w / 2.0f, y + h / 2.0f, velocityX * speed, velocityY * speed, angle({ x, y }, { mouse[0], mouse[1] })));
  }
}

void ClientEntity::renderTick(Renderer& renderer, float deltaTime) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/player.frag");

  float currentDeltaX = this->deltaX(deltaTime) - renderer.cameraPosition[0];
  float currentDeltaY = this->deltaY(deltaTime) - renderer.cameraPosition[1];

  shader->show();
  shader->offset(renderer.cameraPosition);
  shader->entityPosition({ currentDeltaX, currentDeltaY });
  shader->entityDimension({ w, h });

  renderer.drawBuffer.pushSquare(currentDeltaX, currentDeltaY, w, h);
  renderer.drawBuffer.draw(renderer.game, *shader);
  shader->hide();

  vec2f& worldMouse = renderer.game.window->mouseWorld;

  currentDeltaX = worldMouse[0] - renderer.cameraPosition[0];
  currentDeltaY = worldMouse[1] - renderer.cameraPosition[1];

  shader->show();
  shader->offset(renderer.cameraPosition);
  shader->entityPosition({ currentDeltaX, currentDeltaY });
  shader->entityDimension({ 32, 32 });

  renderer.drawBuffer.pushSquare(currentDeltaX + 16, currentDeltaY + 16, 32, 32);
  renderer.drawBuffer.draw(renderer.game, *shader);
  shader->hide();
}
