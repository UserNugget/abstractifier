#include "client/draw/draw_builder.h"
#include "util/lib.h"
#include "client/draw/shader.h"
#include "client/draw/window.h"
#include "client/draw/renderer.h"

static const size_t POSITION = 3 * sizeof(float);
static const size_t COLOR = 1 * sizeof(int);
static const size_t TEXTURE = 2 * sizeof(float);

DrawBuilder::DrawBuilder(Game &game, DrawState state, DrawType type) : game(game), state(state), type(type) {
  vertexIndex = 0;
  vertexSize = 1024 * 64;
  vertexPtr = (char*) malloc(vertexSize);

  glGenBuffers(1, &vertexId);
  glGenBuffers(1, &indexId);
  glGenVertexArrays(1, &arrayId);

  scaleX = game.window->scaleX;
  scaleY = game.window->scaleY;
}

void DrawBuilder::writeable(size_t size) {
  if (vertexIndex + size >= vertexSize) {
    vertexPtr = (char*) realloc(vertexPtr, max(vertexSize * 2, vertexIndex + size) + 1);
  }
}

void DrawBuilder::pushSquare(float x, float y, float w, float h) {
  writeable(POSITION * 6);

  push(x, y + h, 0);
  push(x + w, y + h, 0);
  push(x + w, y, 0);

  push(x + w, y, 0);
  push(x, y, 0);
  push(x, y + h, 0);
}

void DrawBuilder::push(float x, float y, float z) {
  writeable(POSITION);

  float* ptr = (float*) (vertexPtr + vertexIndex);

  ptr[0] = (x / scaleX) - 1.0f;
  ptr[1] = (y / scaleY) - 1.0f;

  ptr[2] = z;
  vertexIndex += POSITION;
}

void DrawBuilder::push(float x, float y, float z, int color) {
  writeable(POSITION + COLOR);

  DrawBuilder::push(x, y, z);

  int* ptr = (int*) (vertexPtr + vertexIndex);
  ptr[0] = color;
  vertexIndex += COLOR;
}

void DrawBuilder::push(float x, float y, float z, int color, float textureX, float textureY) {
  writeable(POSITION + COLOR + TEXTURE);

  DrawBuilder::push(x, y, z, color);

  float* ptr = (float*) (vertexPtr + vertexIndex);
  ptr[0] = textureX;
  ptr[1] = textureY;
  vertexIndex += TEXTURE;
}

void DrawBuilder::draw(Game &game, Shader& shader) {
  shader.resolution(game.window->resolution);
  shader.mouse(game.window->mouse);
  shader.time(game.renderer->time);
  shader.ratio(1.0f);

  glBindVertexArray(arrayId);
  if (state < VERTEX_COLOR) glDisableVertexAttribArray(1); // color
  if (state < VERTEX_COLOR_TEXTURE) glDisableVertexAttribArray(2); // texture

  glEnableVertexAttribArray(0); // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  if (state >= VERTEX_COLOR) {
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, state, vertexPtr + POSITION);
  }
  if (state >= VERTEX_COLOR_TEXTURE) {
    glEnableVertexAttribArray(2); // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, state, vertexPtr + POSITION + COLOR);
  }

  // bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexId);
  glBufferData(GL_ARRAY_BUFFER, vertexIndex, vertexPtr, type);

  // draw
  glDrawArrays(GL_TRIANGLES, 0, vertexIndex / state);

  vertexIndex = 0;
  glBindVertexArray(0);
}
