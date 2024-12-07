#include "client/texture/texture_manager.h"
#include "util/lib.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager(Game &game) : game(game) {

}

GLuint TextureManager::loadGL(const std::string &path) {
  int width, height, comp;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &comp, 0);
  if (data == nullptr) {
    LOG("failed to load " << path << ": " << stbi_failure_reason());
    return 0;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return texture;
}

GLFWimage* TextureManager::load(const std::string& path) {
  GLFWimage* image = (GLFWimage*) malloc(sizeof(GLFWimage));

  int comp = 0;
  image->pixels = stbi_load(path.c_str(), &image->width, &image->height, &comp, 0);
  if (image->pixels == nullptr) {
    LOG("failed to load " << path << ": " << stbi_failure_reason());
    free(image);
    return nullptr;
  }

  return image;
}

void TextureManager::free(GLFWimage* image) {
  if (image->pixels != nullptr) {
    stbi_image_free(image->pixels);
  }

  std::free(image);
}
