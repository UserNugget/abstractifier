#include "client/draw/shader.h"
#include <sstream>
#include <fstream>

GLuint compile(std::string& path, GLuint type) {
  std::ifstream file(path);
  if (!file) {
    LOG("failed to open " << path)
    return -1;
  }

  std::stringstream stream;
  stream << file.rdbuf();
  std::string source = stream.str();
  const char *sourcePtr = source.c_str();

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &sourcePtr, nullptr);
  glCompileShader(shader);

  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    char *message = (char*) malloc(length);
    glGetShaderInfoLog(shader, length, nullptr, message);
    LOG(path << " shader compilation failed:\n" << message)
    free(message);

    glDeleteShader(shader);
    return -1;
  }

  return shader;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
  GLuint vertex = compile(vertexPath, GL_VERTEX_SHADER);
  GLuint fragment = compile(fragmentPath, GL_FRAGMENT_SHADER);

  LOG("compiled " << fragmentPath)

  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  GLint linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    char* message = (char*) malloc(length * sizeof(char));
    glGetProgramInfoLog(program, length, nullptr, message);
    LOG("failed to link shader:\n" << message)
    free(message);

    glDeleteProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return;
  }

  glDetachShader(program, vertex);
  glDetachShader(program, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "color");

  timeIndex = glGetUniformLocation(program, "time");
  resolutionIndex = glGetUniformLocation(program, "resolution");
  mouseIndex = glGetUniformLocation(program, "mouse");
  offsetIndex = glGetUniformLocation(program, "offset");
  ratioIndex = glGetUniformLocation(program, "ratio");
  entityPositionIndex = glGetUniformLocation(program, "entityPosition");
  entityDimensionIndex = glGetUniformLocation(program, "entityDimension");
  frameRateIndex = glGetUniformLocation(program, "frameRate");
  tickTimeIndex = glGetUniformLocation(program, "tickTime");
  scoreIndex = glGetUniformLocation(program, "score");
}

void Shader::show() const {
  glUseProgram(program);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void Shader::hide() const {
  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Shader::time(float time) const {
  if (timeIndex == GL_INVALID_INDEX) return;
  glUniform1f(timeIndex, time);
}

void Shader::resolution(vec2i resolution) const {
  if (resolutionIndex == GL_INVALID_INDEX) return;
  glUniform2f(resolutionIndex, (GLfloat) resolution[0], (GLfloat) resolution[1]);
}

void Shader::mouse(vec2i mouse) const {
  if (mouseIndex == GL_INVALID_INDEX) return;
  glUniform2f(mouseIndex, (GLfloat) mouse[0], (GLfloat) mouse[1]);
}

void Shader::offset(vec2f offset) const {
  if (offsetIndex == GL_INVALID_INDEX) return;
  glUniform2f(offsetIndex, (GLfloat) offset[0], (GLfloat) offset[1]);
}

void Shader::ratio(float ratio) const {
  if (ratioIndex == GL_INVALID_INDEX) return;
  glUniform1f(ratioIndex, ratio);
}

void Shader::entityPosition(vec2f coords) const {
  if (entityPositionIndex == GL_INVALID_INDEX) return;
  glUniform2f(entityPositionIndex, coords[0], coords[1]);
}

void Shader::entityDimension(vec2f coords) const {
  if (entityDimensionIndex == GL_INVALID_INDEX) return;
  glUniform2f(entityDimensionIndex, coords[0], coords[1]);
}

void Shader::frameRate(float frameRate) const {
  if (frameRateIndex == GL_INVALID_INDEX) return;
  glUniform1f(frameRateIndex, frameRate);
}

void Shader::tickTime(float tickTime) const {
  if (tickTimeIndex == GL_INVALID_INDEX) return;
  glUniform1f(tickTimeIndex, tickTime);
}

void Shader::score(float score) const {
  if (scoreIndex == GL_INVALID_INDEX) return;
  glUniform1f(scoreIndex, score);
}
