#ifndef ABSTRACTIFIER_RENDERER_H
#define ABSTRACTIFIER_RENDERER_H

#include "client/world/client_world.h"
#include "client/draw/draw_builder.h"

class Renderer {
public:
  Game& game;
  DrawBuilder drawBuffer = DrawBuilder(game, VERTEX, STATIC);

  float time;
  float frameRate;
  vec2f cameraPosition;

  explicit Renderer(Game &game);

  void draw();
};

#endif