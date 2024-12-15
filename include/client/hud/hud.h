#ifndef ABSTRACTIFIER_HUD_H
#define ABSTRACTIFIER_HUD_H

#include "client/draw/renderer.h"

class Hud {
public:
  Renderer& renderer;
  uint64_t startAt = 0;

  explicit Hud(Renderer &renderer);

  void draw();
};

#endif