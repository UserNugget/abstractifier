#ifndef ABSTRACTIFIER_ENEMY_H
#define ABSTRACTIFIER_ENEMY_H

#include "entity.h"

class Enemy : public Entity {
private:
  Enemy(float x, float y, float w, float h);

public:
  void tick(World &world) override;

  void remove(World &world) override;

  static Enemy* allocate(float x, float y, float w, float h);
};


#endif //ABSTRACTIFIER_ENEMY_H
