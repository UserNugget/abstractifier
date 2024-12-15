#ifndef ABSTRACTIFIER_MAIN_MENU_H
#define ABSTRACTIFIER_MAIN_MENU_H

#include "client/menu/menu.h"

class MainMenu : public Menu {
public:
  float closeAt = 0;

  explicit MainMenu(Renderer &renderer);

  ~MainMenu() override;

  void draw() override;
};


#endif
