#ifndef ABSTRACTIFIER_PAUSE_MENU_H
#define ABSTRACTIFIER_PAUSE_MENU_H

#include "client/menu/menu.h"

class PauseMenu : public Menu {
public:
  float closeAt = 0;
  explicit PauseMenu(Renderer &renderer);

  ~PauseMenu() override;

  void draw() override;
};


#endif
