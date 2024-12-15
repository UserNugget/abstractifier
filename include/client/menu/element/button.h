#ifndef ABSTRACTIFIER_BUTTON_H
#define ABSTRACTIFIER_BUTTON_H

#include "client/menu/element/label.h"

class Button : public Label {
  typedef void (*action_func)(Renderer&, float, float);

public:
  action_func action;
  float hoveredDelta;

  Button(Renderer& renderer, float x, float y, float w, float h, std::string &text, int color,
         Button::action_func action);

  void draw() override;
  void clicked(float x, float y) override;
};

#endif
