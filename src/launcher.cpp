#include "client/input/input.h"
#include "client/draw/window.h"
#include "game.h"

int main() {
  Game game;
  game.tick();
  game.render();

  return 0;
}
