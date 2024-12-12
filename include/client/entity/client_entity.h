#ifndef ABSTRACTIFIER_CLIENT_ENTITY_H
#define ABSTRACTIFIER_CLIENT_ENTITY_H

#include "world/entity.h"

class Input;

class ClientEntity : public Entity {
public:
  Input* input;
  int overdriveTicks = 0;

  ClientEntity(float x, float y, float w, float h, Input* input);

  void tick(World& world) override;
  void renderTick(Renderer& renderer, float deltaTime) override;

  static void draw(Renderer& renderer);
};


#endif //ABSTRACTIFIER_CLIENT_ENTITY_H
