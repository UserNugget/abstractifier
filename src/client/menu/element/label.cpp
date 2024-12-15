#include "client/menu/element/label.h"

Label::Label(Renderer &renderer, float x, float y, float w, float h, std::string &text, int color)
    : Element(renderer, x, y, w, h), text(text), color(color) {
  state |= DRAW_SHADOW;
}

void Label::draw() {
  renderer.font.draw(x + w / 2.0f, y + h / 2.0f + renderer.font.height - 8, state, color, text);
}
