#ifndef ABSTRACTIFIER_INPUT_H
#define ABSTRACTIFIER_INPUT_H

#include "game.h"

class Input {
public:
  Game& game;

  bool shoot = false;
  bool spreadShoot = false;
  bool forward = false, backward = false;
  bool left = false, right = false;
  float scale = 10;

  explicit Input(Game& game);

  void onPress(int key, int scancode, int action, int mods);
};

#endif
