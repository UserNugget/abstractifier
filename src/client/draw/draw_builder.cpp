#include "client/draw/draw_builder.h"
#include "util/lib.h"
#include "client/draw/shader.h"
#include "client/draw/window.h"
#include "client/draw/renderer.h"

static const int64_t POSITION = 2 * sizeof(float);
static const int64_t COLOR = 1 * sizeof(int);
static const int64_t TEXTURE = 2 * sizeof(float);

DrawBuilder::DrawBuilder(Game &game, DrawState state, DrawType type) : DrawBuilder(game, state, type, GL_TRIANGLES) { }

DrawBuilder::DrawBuilder(Game &game, DrawState state, DrawType type, GLint glType)
    : game(game), state(state), type(type), glType(glType) {
  vertexIndex = 0;
  vertexSize = 64;
  vertexPtr = (char*) malloc(vertexSize);

  glGenBuffers(1, &vertexId);
  glGenVertexArrays(1, &arrayId);

  scaleX = game.window->scaleX;
  scaleY = game.window->scaleY;
}

DrawBuilder::~DrawBuilder() {
  glDeleteBuffers(1, &vertexId);
  glDeleteVertexArrays(1, &arrayId);
  free(vertexPtr);
}

void DrawBuilder::writeable(int64_t size) {
  if (vertexIndex + size >= vertexSize) {
    vertexSize = std::max(vertexSize * 2, vertexIndex + size + 1);
    vertexPtr = (char*) realloc(vertexPtr, vertexSize);
  }
}

void DrawBuilder::pushSquare(float x, float y, float w, float h) {
  writeable(POSITION * 6);

  push(x, y + h);
  push(x + w, y + h);
  push(x + w, y);

  push(x + w, y);
  push(x, y);
  push(x, y + h);
}

void DrawBuilder::pushSquare(float x, float y, float w, float h, int color) {
  writeable((POSITION + COLOR) * 6);

  push(x, y + h, color);
  push(x + w, y + h, color);
  push(x + w, y, color);

  push(x + w, y, color);
  push(x, y, color);
  push(x, y + h, color);
}

void DrawBuilder::push(float x, float y) {
  writeable(VERTEX);

  float* ptr = (float*) (vertexPtr + vertexIndex);

  ptr[0] = (x / scaleX) - 1.0f;
  ptr[1] = (y / scaleY) - 1.0f;

  vertexIndex += POSITION;
}

void DrawBuilder::push(float x, float y, int color) {
  writeable(VERTEX_COLOR);

  push(x, y);

  int* ptr = (int*) (vertexPtr + vertexIndex);
  ptr[0] = color;
  vertexIndex += COLOR;
}

void DrawBuilder::push(float x, float y, int color, float textureX, float textureY) {
  writeable(VERTEX_COLOR_TEXTURE);

  push(x, y, color);

  float* ptr = (float*) (vertexPtr + vertexIndex);
  ptr[0] = textureX;
  ptr[1] = textureY;
  vertexIndex += TEXTURE;
}

void DrawBuilder::draw(Shader& shader) {
  if ((vertexIndex % state) != 0) {
    LOG("invalid buffer state")
  }

  shader.resolution(game.window->resolution);
  shader.mouse(game.window->mouse);
  shader.time(game.renderer->time);

  glBindVertexArray(arrayId);
  shader.disableAttributes(state);

  glBindBuffer(GL_ARRAY_BUFFER, vertexId);
  glBufferData(GL_ARRAY_BUFFER, vertexIndex, vertexPtr, type);
  shader.enableAttributes(state);

  glDrawArrays(glType, 0, vertexIndex / state);

  vertexIndex = 0;
  glBindVertexArray(0);
}

bool DrawBuilder::empty() {
  return vertexIndex == 0;
}
