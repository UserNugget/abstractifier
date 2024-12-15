#include "client/draw/window.h"
#include "client/menu/overlay/overlay.h"
#include "client/menu/type/pause_menu.h"
#include "client/menu/element/button.h"
#include "client/menu/element/label.h"
#include "client/menu/type/main_menu.h"

static std::string pauseMessage = "Pause";
static std::string continueMessage = "Continue";
static std::string backMessage = "Back to menu";
static std::string exitMessage = "Exit";

PauseMenu::PauseMenu(Renderer &renderer) : Menu(renderer) {
  renderer.game.world->paused = true;
  vec2i& resolution = renderer.game.window->expectedResolution;

  float width = 256;
  float height = 64;
  float x = resolution.x() / 2.0f - width / 2.0f;
  float y = resolution.y() / 2.0f + height - 24;

  Label* pause = new Label(renderer, x, y + renderer.font.height + 24, width, height, pauseMessage, LIB_RGB(0, 255, 0));
  pause->state |= DRAW_BACKGROUND;
  elements.emplace_back(pause);

  elements.emplace_back(new Button(renderer, x, y, width, height, continueMessage, LIB_RGB(0, 0, 0), [](Renderer& renderer, float x, float y) {
    if (static_cast<PauseMenu*>(renderer.menu)->closeAt == 0) {
      renderer.closeMenu();
    }
  }));

  elements.emplace_back(new Button(renderer, x, y - (height + 12), width, height, backMessage, LIB_RGB(0, 0, 0), [](Renderer& renderer, float x, float y) {
    PauseMenu* menu = static_cast<PauseMenu*>(renderer.menu);
    if (menu->closeAt == 0) {
      renderer.openOverlay(new Overlay(renderer));
      menu->closeAt = renderer.time + (float) renderer.overlay->duration / 2000.0f;
    }
  }));

  elements.emplace_back(new Button(renderer, x, y - (height + 12) * 2, width, height, exitMessage, LIB_RGB(0, 0, 0), [](Renderer& renderer, float x, float y) {
    exit(0);
  }));
}

PauseMenu::~PauseMenu() {
  renderer.game.world->paused = false;
}

void PauseMenu::draw() {
  Menu::draw();

  if (closeAt != 0 && renderer.time >= closeAt) {
    renderer.game.world->at(0)->type = HUD_PLAYER;
    renderer.game.world->gameOver();
    renderer.openMenu(new MainMenu(renderer));
  }
}
