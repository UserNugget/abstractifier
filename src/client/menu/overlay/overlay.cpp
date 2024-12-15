#include "client/menu/overlay/overlay.h"
#include "client/draw/window.h"

Overlay::Overlay(Renderer &renderer) : renderer(renderer) {

}

void Overlay::draw() {
  uint64_t timePassed = timeMillis() - start;

  float progress = (float) timePassed / ((float) duration / 2.0f);

  static Shader* shader = new Shader("shaders/overlay/overlay.vert", "shaders/overlay/overlay.frag");
  static DrawBuilder drawer = DrawBuilder(renderer.game, VERTEX_COLOR, STATIC);
  static GLint progressUniform = glGetUniformLocation(shader->program, "progress");

  glEnable(GL_BLEND);
  shader->show();
  if (progressUniform != -1) {
    glUniform1f(progressUniform, progress);
  }
  drawer.pushSquare(0, 0, (float) renderer.game.window->expectedResolution.x(), (float) renderer.game.window->expectedResolution.y(), LIB_RGB(0, 0, 0));
  drawer.draw(*shader);
  shader->hide();
  glDisable(GL_BLEND);

  if (timePassed >= duration) {
    renderer.closeOverlay();
  }
}
