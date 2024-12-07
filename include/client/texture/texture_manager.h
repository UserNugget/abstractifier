#ifndef ABSTRACTIFIER_TEXTURE_MANAGER_H
#define ABSTRACTIFIER_TEXTURE_MANAGER_H

#include <string>
#include "game.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

class TextureManager {
public:
  Game& game;

  explicit TextureManager(Game &game);

  GLuint loadGL(const std::string& path);
  GLFWimage* load(const std::string& path);
  void free(GLFWimage* image);
};


#endif //ABSTRACTIFIER_TEXTURE_MANAGER_H
