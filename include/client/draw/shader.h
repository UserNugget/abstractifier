#ifndef ABSTRACTIFIER_SHADER_H
#define ABSTRACTIFIER_SHADER_H

#include "glad/gl.h"
#include "math/vectors.h"

class Shader {
public:
  GLuint program;

  GLint resolutionIndex;
  GLint timeIndex;
  GLint mouseIndex;
  GLint offsetIndex;
  GLint ratioIndex;
  GLint entityPositionIndex;
  GLint entityDimensionIndex;
  GLint frameRateIndex;
  GLint tickTimeIndex;
  GLint scoreIndex;

  Shader(std::string vertexPath, std::string fragmentPath);

  void show() const;
  void hide() const;

  void resolution(vec2i resolution) const;
  void time(float time) const;
  void mouse(vec2i mouse) const;
  void offset(vec2f offset) const;
  void ratio(float ratio) const;
  void entityPosition(vec2f coords) const;
  void entityDimension(vec2f coords) const;
  void frameRate(float frameRate) const;
  void tickTime(float tickTime) const;
  void score(float score) const;
};

#endif
