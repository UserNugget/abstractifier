#ifndef ABSTRACTIFIER_CLIENT_ENTITY_H
#define ABSTRACTIFIER_CLIENT_ENTITY_H

#include <list>
#include "world/entity.h"

class Input;

class ClientGun {
public:
  float gunsDistance = 72.0f;

  float angle = 0;
  float correctingAngle = -1;
  float oldAngle = 0;

  float distance = 0;
  float oldDistance = 0;

  ClientGun(float angle);

  void tick();
};

class ClientEntity : public Entity {
public:
  Input* input;
  int overdriveTicks = 0;
  Entity* enemy = nullptr;

  std::list<ClientGun> guns {};

  ClientEntity(float x, float y, float w, float h, Input* input);

  void tick(World& world) override;
  void renderTick(Renderer& renderer, float deltaTime) override;

  void reset() override;

  static void draw(Renderer& renderer);

  void initGuns();
  void refreshGuns();
};


#endif
