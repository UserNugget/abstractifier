#ifndef ABSTRACTIFIER_DRAW_BUILDER_H
#define ABSTRACTIFIER_DRAW_BUILDER_H

#include "glad/gl.h"
#include "shader.h"
#include "game.h"

enum DrawState {
  VERTEX = 3 * sizeof(float), VERTEX_COLOR = VERTEX + sizeof(int), VERTEX_COLOR_TEXTURE = VERTEX_COLOR + (2 * sizeof(float))
};

enum DrawType {
  STATIC = GL_STATIC_DRAW,
  DYNAMIC = GL_DYNAMIC_DRAW
};

class DrawBuilder {
public:
  Game& game;
  DrawState state;
  DrawType type;

  float scaleX, scaleY;

  GLuint vertexId, arrayId;

  char* vertexPtr;
  int64_t vertexIndex;
  int64_t vertexSize;

  explicit DrawBuilder(Game& game, DrawState state, DrawType type);

  void writeable(int64_t size);
  void pushSquare(float x, float y, float w, float h);
  void push(float x, float y, float z);
  void push(float x, float y, float z, int color);
  void push(float x, float y, float z, int color, float textureX, float textureY);
  void draw(Game& game, Shader& shader);
};


#endif //ABSTRACTIFIER_DRAW_BUILDER_H
