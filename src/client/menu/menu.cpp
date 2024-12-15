#include "client/draw/window.h"
#include "client/menu/element/element.h"
#include "client/menu/menu.h"

Menu::Menu(Renderer &renderer) : renderer(renderer) {

}

Menu::~Menu() {
  for (Element* element : elements) {
    delete element;
  }
}

void Menu::draw() {
  for (Element* element : elements) {
    element->draw();
  }
}

Element *Menu::findAt(float x, float y) {
  x = scaleX(x);
  y = scaleY(y);

  for (Element* element : elements) {
    if (element->x <= x && element->x + element->w >= x &&
        element->y <= y && element->y + element->h >= y) {
      return element;
    }
  }

  return nullptr;
}

void Menu::hover(float x, float y) {
  if (hovered != nullptr) {
    hovered->hover = false;
  }

  hovered = findAt(x, y);
  if (hovered != nullptr && !hovered->hover) {
    hovered->hover = true;
    hovered->hovered(scaleX(x), scaleX(y));
  }
}

void Menu::click(float x, float y) {
  Element* element = findAt(x, y);
  if (element != nullptr) {
    element->clicked(scaleX(x), scaleY(y));
  }
}

float Menu::scaleX(float x) {
  Window* window = renderer.game.window;
  return ((x - window->viewport[0]) / window->viewport[2]) * (float) window->expectedResolution.x();
}

float Menu::scaleY(float y) {
  Window* window = renderer.game.window;
  return ((y - window->viewport[1]) / window->viewport[3]) * (float) window->expectedResolution.y();
}

void Menu::closed() {

}
