#ifndef ABSTRACTIFIER_OVERLAY_H
#define ABSTRACTIFIER_OVERLAY_H

#include "client/draw/draw_builder.h"
#include "client/draw/renderer.h"
#include "client/draw/shader.h"

class Overlay {
public:
  Renderer& renderer;
  uint64_t start = timeMillis();
  uint64_t duration = 3000;

  explicit Overlay(Renderer& renderer);

  virtual void draw();
};

#endif
