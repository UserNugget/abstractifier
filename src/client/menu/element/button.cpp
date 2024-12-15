#include "client/menu/element/button.h"

Button::Button(Renderer &renderer, float x, float y, float w, float h, std::string &text, int color,
               Button::action_func action) : Label(renderer, x, y, w, h, text, color), action(action) {

}

void Button::draw() {
  if (hover) {
    hoverAt = timeMillis();
  }

  static DrawBuilder buffer = DrawBuilder(renderer.game, VERTEX_COLOR, STATIC);
  static Shader* button = new Shader("shaders/menu/button.vert", "shaders/menu/button.frag");
  static Shader* outline = new Shader("shaders/menu/outline.vert", "shaders/menu/outline.frag");

  float hoverDelta = ((float) (timeMillis() - hoverAt) / 500.0f) * 100.0f;
  float factor = (100.0f + hoverDelta) / 70.0f;

  int hoverColor = (int) std::min(68.0f / factor, 255.0f);

  button->show();
  buffer.pushSquare(x, y, w, h, LIB_RGB(hoverColor, hoverColor, hoverColor));
  buffer.draw(*button);
  button->hide();

  outline->show();
  buffer.pushSquare(x - 6, y, w + 12, 6, LIB_RGB(0, 0, 0));
  buffer.pushSquare(x - 6, y + h - 6, w + 12, 6, LIB_RGB(0, 0, 0));
  buffer.draw(*outline);
  outline->hide();

  color = LIB_RGB(244, 244, 244);

  Label::draw();
}

void Button::clicked(float x, float y) {
  action(renderer, x, y);
}
