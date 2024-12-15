#ifndef ABSTRACTIFIER_SHADER_H
#define ABSTRACTIFIER_SHADER_H

#include "glad/gl.h"
#include "math/vectors.h"
#include "draw_builder.h"

class Shader {
public:
  GLuint program;

  GLint positionId, colorId, textureId;

  GLint resolutionIndex;
  GLint timeIndex;
  GLint mouseIndex;
  GLint offsetIndex;

  Shader(std::string vertexPath, std::string fragmentPath);

  void show() const;
  void hide() const;

  void enableAttributes(DrawState state);
  void disableAttributes(DrawState state);

  void resolution(vec2i resolution) const;
  void time(float time) const;
  void mouse(vec2i mouse) const;
  void offset(vec2f offset) const;
};

#endif
