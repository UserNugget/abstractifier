#ifndef ABSTRACTIFIER_FONT_H
#define ABSTRACTIFIER_FONT_H

#include "glad/gl.h"
#include "math/vectors.h"
#include "stb_truetype.h"
#include <string>

class Renderer;

enum RenderState {
  ALIGN_CENTER = 1 << 0,
  ALIGN_RIGTH = 1 << 1,
  DRAW_BACKGROUND = 1 << 2,
  DRAW_INVERT = 1 << 3,
  DRAW_UNDERLINE = 1 << 4,
  DRAW_SHADOW = 1 << 5
};

class Font {
private:
  stbtt_bakedchar* glyphs;

public:
  Renderer& renderer;

  GLuint fontTexture;
  float height;

  Font(Renderer& renderer, const std::string& filePath, float height);

  vec2f draw(float x, float y, int state, int color, const std::string &message);
  float width(const std::string& message, int from);

  void applyAlignment(const std::string& message, int from, vec2f& position, int state);
  bool applyEffect(wchar_t type, int& state);
  stbtt_bakedchar& findGlyph(wchar_t glyph);
};


#endif
