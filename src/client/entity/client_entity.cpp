#include "client/entity/client_entity.h"
#include "client/input/input.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include "world/bullet.h"
#include <limits>

ClientEntity::ClientEntity(float x, float y, float w, float h, Input* input) : Entity(x, y, w, h), input(input) {
  type = HUD_PLAYER;
  initGuns();
}

void ClientEntity::tick(World& world) {
  bool shoot = input->shoot;
  bool spreadShoot = input->spreadShoot;
  Entity::tick(world);

  float speedX = 0;
  float speedY = 0;

  if (input->forward) speedY++;
  if (input->right) speedX++;
  if (input->backward) speedY--;
  if (input->left) speedX--;

  if (type == HUD_PLAYER) {
    shoot = false;
    spreadShoot = false;

    if (enemy == nullptr || enemy->removed || (ticks % 5 == 0)) {
      float distance = std::numeric_limits<float>::max();;
      for (Entity *entity: *world.entities) {
        if (entity->type == ENEMY && !entity->removed) {
          float distanceToEnemy = distanceSquared(*entity);
          if (enemy == nullptr && distanceToEnemy > 512 * 512) {
            continue;
          }

          if (distanceToEnemy < distance) {
            distance = distanceToEnemy;
            enemy = entity;
          }
        }
      }
    }

    if (enemy != nullptr) {
      float angle = vec2f { enemy->x, enemy->y }.angle({ x, y });
      spreadShoot = guns.size() >= 3;

      float distance = distanceSquared(*enemy);
      if (distance > 256 * 256) {
        velocityX = sinf(angle) * 2.0f;
        velocityY = cosf(angle) * 2.0f;
      } else {
        velocityX *= 0.5;
        velocityY *= 0.5;
      }

      world.game.window->mouseWorld = vec2f { enemy->x - enemy->w / 2.0f, enemy->y - enemy->h / 2.0f };
      shoot = distance <= 512 * 512;
      speed = 10;
    }
  } else {
    velocityX = speedX;
    velocityY = speedY;
    speed = input->scale;
  }

  if (world.abilityScore > 600) {
    overdriveTicks += 120;
    world.abilityScore -= 600;

    if (guns.size() < 5) {
      guns.emplace_back(-1);
      refreshGuns();
    }
  }

  if (overdriveTicks > 0) {
    overdriveTicks--;
  }

  bool correctingAngle = false;
  bool angleCorrected = true;
  for (ClientGun& gun : guns) {
    gun.tick();
    if (gun.correctingAngle != 0) {
      correctingAngle = true;
      if (std::abs(gun.correctingAngle - gun.angle) > 0.1f) {
        angleCorrected = false;
      }
    }
  }

  if (correctingAngle && angleCorrected) {
    for (ClientGun& gun : guns) {
      gun.angle = gun.correctingAngle;
      gun.correctingAngle = -1;
    }
  }

  if ((shoot || spreadShoot) && (ticks % 2) == 0) {
    vec2f& mouse = world.game.window->mouseWorld;

    for (ClientGun& gun : guns) {
      float angle = gun.angle;
      float x = this->x + sinf(angle) * gun.distance;
      float y = this->y + cosf(angle) * gun.distance;
      float rotation;
      if (spreadShoot) {
        rotation = mouse.angle({ this->x, this->y }) + RND_RADIANS;
      } else {
        rotation = mouse.angle({ x, y }) + RND_RADIANS;
      }

      world.add(Bullet::allocate(x, y, velocityX * speed, velocityY * speed, rotation));
    }
  }
}

void ClientEntity::renderTick(Renderer& renderer, float deltaTime) {
  float deltaX = this->cameraDeltaX(renderer, deltaTime);
  float deltaY = this->cameraDeltaY(renderer, deltaTime);
  renderer.playerBuffer.pushSquare(deltaX, deltaY, w, h, LIB_RGB(9, 12, 250));

  std::list<ClientGun> gunsCopy = guns;
  for (ClientGun& gun: gunsCopy) {
    float distance = lerp(gun.oldDistance, gun.distance, deltaTime);
    float angle = lerp(gun.oldAngle, gun.angle, deltaTime);

    float x = sinf(angle) * distance;
    float y = cosf(angle) * distance;

    renderer.playerBuffer.pushSquare(deltaX + w / 2.0f + x - 8, deltaY + h / 2.0f + y - 8, 16, 16, LIB_RGB(0, 0, 0));
  }
}

void ClientEntity::draw(Renderer &renderer) {
  static Shader* shader = new Shader("shaders/scale.vert", "shaders/entity/player.frag");
  if (renderer.playerBuffer.empty()) {
    return;
  }

  shader->show();
  shader->offset(renderer.cameraPosition);
  renderer.playerBuffer.draw(*shader);
  shader->hide();
}

void ClientEntity::reset() {
  Entity::reset();
  guns.clear();
  initGuns();
  overdriveTicks = 0;
}

void ClientEntity::refreshGuns() {
  float angleDelta = radians(360) / guns.size();
  float angleStart = 0;
  for (ClientGun& gun : guns) {
    if (gun.angle == -1) {
      gun.angle = angleStart;
      gun.oldAngle = angleStart;
      gun.correctingAngle = angleStart;
    } else {
      gun.correctingAngle = angleStart;
    }

    angleStart += angleDelta;
  }
}

void ClientEntity::initGuns() {
  guns.emplace_back(0);
  guns.emplace_back(0);

  refreshGuns();

  for (ClientGun& gun : guns) {
    gun.angle = gun.correctingAngle;
    gun.oldAngle = gun.correctingAngle;
    gun.correctingAngle = -1;
  }
}

ClientGun::ClientGun(float angle) : angle(angle), oldAngle(angle) {

}

void ClientGun::tick() {
  if (distance != gunsDistance) {
    oldDistance = distance;
    distance += (gunsDistance - distance) * 0.05f;
    if (std::abs(gunsDistance - distance) < 0.1f) {
      oldDistance = distance = gunsDistance;
    }
  }

  oldAngle = angle;
  if (correctingAngle != -1) {
    angle += (correctingAngle - angle) * 0.05f;
  } else {
    angle = fmod(angle + radians(1), radians(360));
  }
}
