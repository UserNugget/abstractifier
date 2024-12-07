#ifndef ABSTRACTIFIER_BULLET_H
#define ABSTRACTIFIER_BULLET_H


#include "entity.h"

class Bullet : public Entity {
public:
  float angle;
  float speedX;
  float speedY;

  Bullet(float x, float y, float speedX, float speedY, float angle);

  void tick(World &world) override;

  void renderTick(Renderer &renderer, float deltaTime) override;
};


#endif //ABSTRACTIFIER_BULLET_H
