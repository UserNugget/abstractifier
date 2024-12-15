#include "client/menu/element/element.h"

Element::Element(Renderer &renderer, float x, float y, float w, float h) : renderer(renderer), x(x), y(y), w(w), h(h) {

}

void Element::draw() {

}

void Element::clicked(float x, float y) {

}

void Element::hovered(float x, float y) {
  hoverAt = timeMillis();
}
