#ifndef ABSTRACTIFIER_ENEMY_H
#define ABSTRACTIFIER_ENEMY_H

#include "entity.h"

class Enemy : public Entity {
public:
  Enemy(float x, float y, float w, float h);

  void tick(World &world) override;
};


#endif //ABSTRACTIFIER_ENEMY_H
