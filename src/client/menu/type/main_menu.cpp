#include "client/menu/type/main_menu.h"
#include "client/menu/element/button.h"
#include "client/menu/overlay/overlay.h"

static std::string exitMessage = "Exit";
static std::string playMessage = "Play";

MainMenu::MainMenu(Renderer &renderer) : Menu(renderer) {
  closeable = false;

  elements.emplace_back(new Button(renderer, 32, 32 + (72 * 1), 256, 64, playMessage, LIB_RGB(0, 0, 0), [](Renderer& renderer, float x, float y) {
    MainMenu* menu = static_cast<MainMenu*>(renderer.menu);
    if (menu->closeAt != 0) {
      return;
    }

    renderer.openOverlay(new Overlay(renderer));
    menu->closeAt = renderer.time + (float) renderer.overlay->duration / 2000.0f;
  }));

  elements.emplace_back(new Button(renderer, 32, 32 + (72 * 0), 256, 64, exitMessage, LIB_RGB(0, 0, 0), [](Renderer& renderer, float x, float y) {
    exit(0);
  }));
}

MainMenu::~MainMenu() {
  renderer.game.world->at(0)->type = PLAYER;
  renderer.game.world->gameOver();
}

void MainMenu::draw() {
  Menu::draw();

  if (closeAt != 0 && renderer.time >= closeAt) {
    renderer.closeMenu();
  }
}
