#ifndef ABSTRACTIFIER_GAME_H
#define ABSTRACTIFIER_GAME_H

class Window;
class Input;
class Renderer;
class TextureManager;
class World;

class Game {
public:
  Window* window;
  Input* input;
  Renderer* renderer;
  TextureManager* textureManager;
  World *world;

  bool quit = false;

  Game();
  ~Game();

  void render();
  void tick();
};

#endif
