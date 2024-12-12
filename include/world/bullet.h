#ifndef ABSTRACTIFIER_BULLET_H
#define ABSTRACTIFIER_BULLET_H


#include "entity.h"

class Bullet : public Entity {
public:
  float angle;
  float speedX;
  float speedY;

private:
  Bullet(float x, float y, float speedX, float speedY, float angle);

public:
  void tick(World &world) override;

  void renderTick(Renderer &renderer, float deltaTime) override;

  void remove(World &world) override;

  static Bullet* allocate(float x, float y, float speedX, float speedY, float angle);
};


#endif //ABSTRACTIFIER_BULLET_H
