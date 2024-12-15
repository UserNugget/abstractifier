#ifndef ABSTRACTIFIER_RENDERER_H
#define ABSTRACTIFIER_RENDERER_H

#include "client/world/client_world.h"
#include "client/draw/draw_builder.h"
#include "client/draw/shader.h"
#include "client/draw/font.h"

class Hud;
class Menu;
class Overlay;

class Renderer {
public:
  Game& game;
  Hud* hud = nullptr;
  Menu* menu = nullptr;
  Overlay* overlay = nullptr;

  DrawBuilder entityBuffer = DrawBuilder(game, VERTEX_COLOR, STATIC);
  DrawBuilder playerBuffer = DrawBuilder(game, VERTEX_COLOR, STATIC);
  DrawBuilder bulletBuffer = DrawBuilder(game, VERTEX, STATIC);
  DrawBuilder drawBuffer = DrawBuilder(game, VERTEX, STATIC);

  Font font = Font(*this, "fonts/text.ttf", 32);

  float time = 0;
  float frameRate = 0;
  int shownObjects = 0;
  vec2f cameraPosition {};

  explicit Renderer(Game &game);
  virtual ~Renderer();

  void draw();

  void drawControl(Entity *client);

  Menu* openMenu(Menu* value);
  void closeMenu();

  Overlay* openOverlay(Overlay* value);
  void closeOverlay();
};

#endif