#ifndef ABSTRACTIFIER_WINDOW_H
#define ABSTRACTIFIER_WINDOW_H

#include <array>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "math/vectors.h"
#include "game.h"

class Window {
public:
  Game& game;
  GLFWwindow* window;
  vec2i windowedResolution { 0, 0 };
  vec2i resolution { 0, 0 };
  vec6f viewpoint { 0, 0, 0, 0, 0, 0 };
  vec2i mouse { 0, 0 };
  vec2f mouseWorld { 0, 0 };

  vec2i expectedResolution { 1280, 720 };
  float scaleX = ((float) expectedResolution[0]) / 2.0f;
  float scaleY = ((float) expectedResolution[1]) / 2.0f;

  explicit Window(Game& game);
  ~Window();

  void rescale(int width, int height);
};

#endif
