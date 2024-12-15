#ifndef ABSTRACTIFIER_ELEMENT_H
#define ABSTRACTIFIER_ELEMENT_H

#include "client/draw/renderer.h"

class Element {
public:
  Renderer& renderer;
  float x, y, w, h;
  bool hover = false;
  uint64_t hoverAt = 0;

  Element(Renderer &renderer, float x, float y, float w, float h);

  virtual void draw();
  virtual void hovered(float x, float y);
  virtual void clicked(float x, float y);
};

#endif
