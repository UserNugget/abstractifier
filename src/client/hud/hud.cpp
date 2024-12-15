#include "client/draw/window.h"
#include "client/hud/hud.h"

static std::string helpMessage = "^IW/A/S/D^O     ^Kmove^L\n"
                                 "^ILMB^O        ^Kshoot^L\n"
                                 "^IRMB^O ^Kspread shoot^L\n"
                                 "^IShift^O     ^Ksprint^L\n"
                                 "^IF11^O   ^Kfullscreen^L\n"
                                 "^IESC^O        ^Kpause^L";

Hud::Hud(Renderer &renderer) : renderer(renderer) {

}

void Hud::draw() {
  uint64_t time = timeMillis();
  if (startAt == 0) {
    startAt = time;
  }

  if (time - startAt <= 60000) {
    float timeLeft = (float) (time - startAt) / 60000.0f;

    static Shader* outline = new Shader("shaders/menu/outline.vert", "shaders/menu/outline.frag");
    static DrawBuilder lineBuffer = DrawBuilder(renderer.game, VERTEX_COLOR, STATIC);

    float x = renderer.game.window->expectedResolution.x();
    float y = renderer.game.window->expectedResolution.y();
    vec2f scale = renderer.font.draw(x, y, DRAW_BACKGROUND | ALIGN_RIGTH, LIB_RGB(255, 255, 255), helpMessage);

    outline->show();
    float offset = scale.y() - scale.x();
    lineBuffer.pushSquare(scale.x() + offset * timeLeft, y - renderer.font.height * 6 - 10, offset, 6, LIB_RGB(0, 0, 0));
    lineBuffer.draw(*outline);
    outline->hide();
  }

  renderer.font.draw(0, (float) renderer.game.window->expectedResolution[1], DRAW_BACKGROUND | DRAW_SHADOW, LIB_RGB(255, 255, 255),
                     "^IFPS^O:^K" + std::to_string((int) renderer.frameRate) + "\n" +
                     "^L^IMSPT^O:^K" + std::to_string(renderer.game.world->tickTime) + "\n" +
                     "^L^IObjects^O:^K" + std::to_string(renderer.shownObjects) + "\n" +
                     "^L^IScore^O:^K" + std::to_string(renderer.game.world->score));
}
