#ifndef ABSTRACTIFIER_DRAW_BUILDER_H
#define ABSTRACTIFIER_DRAW_BUILDER_H

#include "glad/gl.h"
#include "math/vectors.h"
#include "game.h"

enum DrawState {
  VERTEX = 2 * sizeof(float),
  VERTEX_COLOR = VERTEX + sizeof(int),
  VERTEX_COLOR_TEXTURE = VERTEX_COLOR + 2 * sizeof(float)
};

enum DrawType {
  STATIC = GL_STATIC_DRAW,
  DYNAMIC = GL_DYNAMIC_DRAW
};

class Shader;

class DrawBuilder {
public:
  Game& game;
  DrawState state;
  DrawType type;

  float scaleX, scaleY;

  GLint glType;
  GLuint vertexId, arrayId;

  char* vertexPtr;
  int64_t vertexIndex;
  int64_t vertexSize;

  explicit DrawBuilder(Game& game, DrawState state, DrawType type);
  explicit DrawBuilder(Game& game, DrawState state, DrawType type, GLint glType);

  ~DrawBuilder();

  void writeable(int64_t size);
  void pushSquare(float x, float y, float w, float h);
  void pushSquare(float x, float y, float w, float h, int color);
  void push(float x, float y);
  void push(float x, float y, int color);
  void push(float x, float y, int color, float textureX, float textureY);
  void draw(Shader& shader);

  bool empty();
};


#endif
