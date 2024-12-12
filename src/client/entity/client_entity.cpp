#include "client/entity/client_entity.h"
#include "client/input/input.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include "world/bullet.h"

ClientEntity::ClientEntity(float x, float y, float w, float h, Input* input) : Entity(x, y, w, h), input(input) {
  type = PLAYER;
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

  if (world.abilityScore > 600) {
    overdriveTicks += 120;
    world.abilityScore -= 600;
  }

  if (overdriveTicks > 0) {
    overdriveTicks--;
  }

  if (input->shoot && (ticks % 2) == 0) {
    vec2f& mouse = world.game.window->mouseWorld;
    float rotation = angle({ x, y }, { mouse[0], mouse[1] });

    world.add(Bullet::allocate(x + w / 2.0f, y + h / 2.0f, velocityX * speed, velocityY * speed, rotation));

    if (overdriveTicks > 0) {
      int bullets = std::min(std::max(overdriveTicks / 110, 2), 4);

      float deltaOne = 0;
      float deltaTwo = 0;

      for (int i = 0; i < bullets; ++i) {
        if ((i % 2) == 0) {
          deltaOne += 0.174532;
          world.add(Bullet::allocate(x + w / 2.0f, y + h / 2.0f, velocityX * speed, velocityY * speed, rotation - deltaOne));
        } else {
          deltaTwo += 0.174532;
          world.add(Bullet::allocate(x + w / 2.0f, y + h / 2.0f, velocityX * speed, velocityY * speed, rotation + deltaTwo));
        }
      }
    }
  }
}

void ClientEntity::renderTick(Renderer& renderer, float deltaTime) {
  float currentDeltaX = this->deltaX(deltaTime) - renderer.cameraPosition[0];
  float currentDeltaY = this->deltaY(deltaTime) - renderer.cameraPosition[1];
  renderer.playerBuffer.pushSquare(currentDeltaX, currentDeltaY, w, h);
}

void ClientEntity::draw(Renderer &renderer) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/player.frag");
  if (renderer.playerBuffer.empty()) {
    return;
  }

  shader->show();
  shader->offset(renderer.cameraPosition);
  renderer.playerBuffer.draw(renderer.game, *shader);
  shader->hide();
}
