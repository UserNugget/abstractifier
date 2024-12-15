#define STB_TRUETYPE_IMPLEMENTATION
#include "client/draw/font.h"
#include "client/draw/shader.h"
#include "client/draw/renderer.h"
#include "client/draw/window.h"
#include <fstream>

#define TEXTURE_WIDTH 512
#define TEXTURE_HEIGHT 512
#define FONT_CHAR_MIN 32
#define FONT_CHAR_COUNT (128 - FONT_CHAR_MIN)
#define FONT_EFFECT_CHAR '^'
#define BACKGROUND_COLOR LIB_RGB(51, 51, 51)
#define BACKGROUND_COLOR_ALPHA LIB_RGBA(51, 51, 51, 128)
#define SHADOW_COLOR LIB_RGB(96, 96, 96)

Font::Font(Renderer& renderer, const std::string &filePath, float height) : height(height), renderer(renderer) {
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);
  std::streamsize fileSize = file.tellg();
  const unsigned char* ttfData = (const unsigned char*) calloc(fileSize + 1, sizeof(char));
  file.seekg(0).read((char*) ttfData, fileSize);
  file.close();

  glyphs = (stbtt_bakedchar*) calloc(FONT_CHAR_COUNT + 1, sizeof(stbtt_bakedchar));
  unsigned char* pixels = (unsigned char*) calloc(TEXTURE_WIDTH * TEXTURE_HEIGHT, sizeof(char));
  stbtt_BakeFontBitmap(ttfData, 0, height, pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, FONT_CHAR_MIN, FONT_CHAR_COUNT, glyphs);

  glGenTextures(1, &fontTexture);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  free(pixels);
  free((unsigned char*) ttfData);
}

stbtt_bakedchar& Font::findGlyph(wchar_t glyph) {
  if (glyph < FONT_CHAR_MIN || glyph >= (FONT_CHAR_COUNT + FONT_CHAR_MIN)) {
    return findGlyph('?');
  }

  return glyphs[glyph - FONT_CHAR_MIN];
}

static void pushQuad(DrawBuilder& text, stbtt_aligned_quad& quad, int color) {
  text.push(quad.x0, quad.y0, color, quad.s0, quad.t1);
  text.push(quad.x1, quad.y0, color, quad.s1, quad.t1);
  text.push(quad.x1, quad.y1, color, quad.s1, quad.t0);
  text.push(quad.x1, quad.y1, color, quad.s1, quad.t0);
  text.push(quad.x0, quad.y1, color, quad.s0, quad.t0);
  text.push(quad.x0, quad.y0, color, quad.s0, quad.t1);
}

vec2f Font::draw(float x, float y, int state, int color, const std::string &message) {
  static DrawBuilder foreground = DrawBuilder(renderer.game, VERTEX_COLOR_TEXTURE, STATIC);
  static DrawBuilder background = DrawBuilder(renderer.game, VERTEX_COLOR, STATIC);
  static DrawBuilder effects = DrawBuilder(renderer.game, VERTEX_COLOR, STATIC);

  vec2f position = { x, y };
  applyAlignment(message, 0, position, state);

  float maximumWidth = x;
  float minimumWidth = position.x();

  for (int i = 0; i < message.length(); i++) {
    if (message[i] == FONT_EFFECT_CHAR && i + 1 < message.length() && applyEffect(message[i + 1], state)) {
      i++;
      continue;
    }

    if (message[i] == '\n') {
      maximumWidth = std::max(position.x(), maximumWidth);
      position.x() = x;
      position.y() -= height;
      applyAlignment(message, i + 1, position, state);
      minimumWidth = std::min(position.x(), minimumWidth);
      continue;
    }

    stbtt_aligned_quad q;
    stbtt_bakedchar& glyph = findGlyph(message[i]);
    float offsetX = floor((position.x() + glyph.xoff) + 0.5f);
    float offsetY = floor(position.y() + glyph.yoff - (height + glyph.yoff) - (glyph.y1 - glyph.y0 + glyph.yoff) + 0.5f);

    if ((state & DRAW_UNDERLINE) != 0) {
      offsetY += 5;
    }

    q.x0 = offsetX;
    q.y0 = offsetY;
    q.x1 = offsetX + (float) glyph.x1 - (float) glyph.x0;
    q.y1 = offsetY + (float) glyph.y1 - (float) glyph.y0;

    q.s0 = (float) glyph.x0 / (float) TEXTURE_WIDTH;
    q.t0 = (float) glyph.y0 / (float) TEXTURE_WIDTH;
    q.s1 = (float) glyph.x1 / (float) TEXTURE_WIDTH;
    q.t1 = (float) glyph.y1 / (float) TEXTURE_WIDTH;

    int fontColor = color;
    if ((state & DRAW_INVERT) != 0) {
      effects.pushSquare(position.x(), position.y() + glyph.yoff - (height + glyph.yoff) - 2, glyph.xadvance, height + 2, color);
      fontColor = BACKGROUND_COLOR;
    }

    if ((state & DRAW_SHADOW) != 0 && (state & DRAW_INVERT) == 0) {
      q.x0 += 2;
      q.x1 += 2;
      q.y0 -= 2;
      q.y1 -= 2;
      pushQuad(foreground, q, SHADOW_COLOR);
      q.x0 -= 2;
      q.x1 -= 2;
      q.y0 += 2;
      q.y1 += 2;
    }

    pushQuad(foreground, q, fontColor);

    if ((state & DRAW_UNDERLINE) != 0) {
      effects.pushSquare(position.x(), position.y() - height, glyph.xadvance, 2, fontColor);
      if ((state & DRAW_SHADOW) != 0 && (state & DRAW_INVERT) == 0) {
        effects.pushSquare(position.x() + 2, position.y() - height - 2, glyph.xadvance, 2, SHADOW_COLOR);
      }
    }

    position.x() += glyph.xadvance;
  }

  maximumWidth = std::max(position.x(), maximumWidth);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  static Shader* text = new Shader("shaders/font/text.vert", "shaders/font/text.frag");
  static Shader* back = new Shader("shaders/font/background.vert", "shaders/font/background.frag");
  static GLint fontTextureUniform = glGetUniformLocation(text->program, "fontTexture");

  back->show();
  if ((state & DRAW_BACKGROUND) != 0) {
    background.pushSquare(minimumWidth, y, maximumWidth - minimumWidth, position.y() - y - height - 4, BACKGROUND_COLOR_ALPHA);
    background.draw(*back);
  }
  effects.draw(*back);
  back->hide();

  text->show();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glUniform1i(fontTextureUniform, 0);
  foreground.draw(*text);
  text->hide();
  glDisable(GL_BLEND);

  return vec2f { minimumWidth, maximumWidth };
}

float Font::width(const std::string& message, int from) {
  int unused = 0;
  float w = 0.0f;
  for (int i = from; i < message.length(); i++) {
    if (message[i] == FONT_EFFECT_CHAR && i + 1 <= message.length() && applyEffect(message[i + 1], unused)) {
      i++;
      continue;
    } else if (message[i] == '\n') {
      break;
    }

    w += findGlyph(message[i]).xadvance;
  }
  return w;
}

bool Font::applyEffect(wchar_t type, int& state) {
  if (type == 'I') {
    state |= DRAW_INVERT;
  } else if (type == 'O') {
    state &= ~DRAW_INVERT;
  } else if (type == 'K') {
    state |= DRAW_UNDERLINE;
  } else if (type == 'L') {
    state &= ~DRAW_UNDERLINE;
  } else if (type == 'N') {
    state |= DRAW_SHADOW;
  } else if (type == 'M') {
    state &= ~DRAW_SHADOW;
  } else {
    return false;
  }

  return true;
}

void Font::applyAlignment(const std::string &message, int from, vec2f &position, int state) {
  if ((state & ALIGN_CENTER) != 0) {
    position.x() -= width(message, from) / 2.0f;
  } else if ((state & ALIGN_RIGTH) != 0) {
    position.x() -= width(message, from);
  }
}
