#ifndef ABSTRACTIFIER_MENU_H
#define ABSTRACTIFIER_MENU_H

#include <vector>
#include "client/draw/renderer.h"

class Element;

class Menu {
public:
  Renderer& renderer;
  std::vector<Element*> elements { };
  Element* hovered = nullptr;
  bool closeable = true;

  explicit Menu(Renderer &renderer);

  virtual ~Menu();

  virtual void draw();
  Element* findAt(float x, float y);

  void hover(float x, float y);
  void click(float x, float y);

  float scaleX(float x);

  float scaleY(float y);

  virtual void closed();
};

#endif
