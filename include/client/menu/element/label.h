#ifndef ABSTRACTIFIER_LABEL_H
#define ABSTRACTIFIER_LABEL_H

#include "client/menu/element/element.h"

class Label : public Element {
public:
  std::string& text;
  int color;
  int state = ALIGN_CENTER;

  Label(Renderer &renderer, float x, float y, float w, float h, std::string &text, int color);

  void draw() override;
};


#endif
