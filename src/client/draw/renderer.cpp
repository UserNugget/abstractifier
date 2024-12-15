#include "game.h"
#include "client/menu/type/main_menu.h"
#include "client/menu/overlay/overlay.h"
#include "client/draw/shader.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include "client/hud/hud.h"
#include "world/bullet.h"

Renderer::Renderer(Game &game) : game(game) {
  hud = new Hud(*this);
  menu = new MainMenu(*this);
}

Renderer::~Renderer() {
  delete hud;
  closeMenu();
  closeOverlay();
}

static float frameDelta = 0;

void Renderer::draw() {
  frameDelta = (float) (timeMillis() - game.world->tickStart) / (1000.0f / (float) TICK_RATE);
  frameDelta = std::max(std::min(frameDelta, 1.0f), 0.0f); // clamp(delta, 0, 1);

  time = (float) glfwGetTime();

  glClearColor(0.5f, 0.5f, 0.5f, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Entity* client = game.world->at(0);
  if (client != nullptr) {
    cameraPosition = { client->deltaX(frameDelta) - game.window->scaleX,
                       client->deltaY(frameDelta) - game.window->scaleY };
  }

  static Shader* background = new Shader("shaders/scale.vert", "shaders/background.frag");
  background->show();
  background->offset(cameraPosition);
  drawBuffer.pushSquare(0, 0, (float) game.window->expectedResolution.x(), (float) game.window->expectedResolution.y());
  drawBuffer.draw(*background);
  background->hide();

  int entities = 0;
  for (Entity* entity: game.world->entitiesSnapshot()) {
    if (!entity->outOfBounds(*this, frameDelta)) {
      entity->renderTick(*this, frameDelta);
      entities++;
    }
  }
  shownObjects = entities;

  Entity::draw(*this);
  ClientEntity::draw(*this);
  Bullet::draw(*this);

  if (client == nullptr) {
    return;
  }

  drawControl(client);

  if (menu != nullptr) {
    menu->draw();
  }

  if (overlay != nullptr) {
    overlay->draw();
  }

  static Shader* mouse = new Shader("shaders/scale.vert", "shaders/mouse/mouse.frag");
  mouse->show();
  mouse->offset(cameraPosition);
  drawBuffer.pushSquare(0, 0, (float) game.window->expectedResolution.x(), (float) game.window->expectedResolution.y());
  drawBuffer.draw(*mouse);
  mouse->hide();
}

void Renderer::drawControl(Entity* client) {
  if (client->type != PLAYER) {
    return;
  }

  vec2i& mouse = game.window->mouse;
  float mouseX = ((2.0f * (((float) mouse.x()) - game.window->viewport[0])) / game.window->viewport[2] - 1.0f) * game.window->scaleX;
  float mouseY = ((2.0f * (((float) mouse.y()) - game.window->viewport[1])) / game.window->viewport[3] - 1.0f) * game.window->scaleY;
  game.window->mouseWorld = { client->deltaX(frameDelta) + mouseX, client->deltaY(frameDelta) + mouseY };

  hud->draw();
}

Menu* Renderer::openMenu(Menu *value) {
  if (menu != nullptr && !menu->closeable) {
    delete value;
    return nullptr;
  }

  closeMenu();
  menu = value;
  return value;
}

void Renderer::closeMenu() {
  if (menu != nullptr) {
    menu->closed();
    delete menu;
    menu = nullptr;
  }
}

Overlay *Renderer::openOverlay(Overlay *value) {
  closeOverlay();
  overlay = value;
  return value;
}

void Renderer::closeOverlay() {
  if (overlay != nullptr) {
    delete overlay;
    overlay = nullptr;
  }
}
