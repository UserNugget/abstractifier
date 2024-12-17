#include "client/input/input.h"
#include "client/draw/window.h"
#include "game.h"

int main() {
  initialize_math();

  Game game;
  game.tick();
  game.render();
  return 0;
}
