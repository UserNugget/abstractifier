#ifndef ABSTRACTIFIER_ENTITY_H
#define ABSTRACTIFIER_ENTITY_H

static int ID_INC = 0;

enum EntityType {
  UNKNOWN, ENEMY, BULLET, PLAYER
};

class Renderer;
class World;

class Entity {
public:
  EntityType type = UNKNOWN;

  int ticks = 0;
  int id = ID_INC++;
  float x, y;
  float w, h;

  float velocityX = 0;
  float velocityY = 0;
  float speed = 1;

  float oldX = 0, oldY = 0;
  bool removed = false;

  Entity(float x, float y, float w, float h);
  ~Entity();

  virtual void tick(World& world);
  virtual void renderTick(Renderer& renderer, float deltaTime);

  virtual void remove(World& world) { }

  void reset() {
    removed = false;
    ticks = 0;
    id = ID_INC++;
  }

  void position(float x, float y) {
    this->x = x;
    this->y = y;
    this->oldX = x;
    this->oldY = y;
  }

  bool outOfBounds(Renderer& renderer, float deltaTime) const;

  float cameraDeltaX(Renderer& renderer, float delta) const;
  float cameraDeltaY(Renderer& renderer, float delta) const;

  float deltaX(float delta) const;
  float deltaY(float delta) const;

  float distanceSqaured(Entity& entity) const {
    float diffX = (x + w / 2.0f) - (entity.x + w / 2.0f);
    float diffY = (y + h / 2.0f) - (entity.y + h / 2.0f);

    return diffX * diffX + diffY * diffY;
  }
};


#endif
