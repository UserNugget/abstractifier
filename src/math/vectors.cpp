#include "math/vectors.h"

#define DEGREES_TO_RADIANS 0.017453292519943295f
#define RADIANS_TO_DEGREES 57.29577951308232f

bool intersect(float fDst1, float fDst2, vec2f& P1, vec2f& P2, vec2f& B1, vec2f& B2, const int point) {
  if (fDst1 == fDst2 || (fDst1 * fDst2) >= 0.0f) return false;
  float hit = P1[point] + (P2[point] - P1[point]) * (-fDst1 / (fDst2 - fDst1));
  return hit > B1[point] && hit < B2[point];
}

bool rayIntersect(vec2f& B1, vec2f& B2, vec2f& L1, vec2f& L2) {
  if ((L2.x() < B1.x() && L1.x() < B1.x())
      || (L2.x() > B2.x() && L1.x() > B2.x())
      || (L2.y() < B1.y() && L1.y() < B1.y())
      || (L2.y() > B2.y() && L1.y() > B2.y())) {
    return false;
  }

  if (L1.x() > B1.x() && L1.x() < B2.x() &&
      L1.y() > B1.y() && L1.y() < B2.y()) {
    return true;
  }

  return intersect(L1.x() - B1.x(), L2.x() - B1.x(), L1, L2, B1, B2, 1)
         || intersect(L1.y() - B1.y(), L2.y() - B1.y(), L1, L2, B1, B2, 0)
         || intersect(L1.x() - B2.x(), L2.x() - B2.x(), L1, L2, B1, B2, 1)
         || intersect(L1.y() - B2.y(), L2.y() - B2.y(), L1, L2, B1, B2, 0);
}

float radians(float angle) {
  return angle * DEGREES_TO_RADIANS;
}

float degrees(float angle) {
  return angle * RADIANS_TO_DEGREES;
}

float lerp(float from, float to, float delta) {
  return from + (to - from) * delta;
}

// http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html and https://gist.github.com/volkansalma/2972237
float atan2_approximate(float y, float x) {
  float r, angle, abs_y = fabs(y) + 1e-10f;
  if (x < 0.0f) {
    r = (x + abs_y) / (abs_y - x);
    angle = 3.0 * M_PI_4;
  } else {
    r = (x - abs_y) / (x + abs_y);
    angle = M_PI_4;
  }
  angle += (0.1963f * r * r - 0.9817f) * r;
  return (y < 0.0f) ? -angle : angle;
}

// https://github.com/kspalaiologos/Maja/blob/trunk/src/main/java/rocks/palaiologos/maja/FastTrigonometry.java
constexpr float PI2 = M_PI * 2;
constexpr int SIN_MASK = 16383;
constexpr int SIN_COUNT = SIN_MASK + 1;
constexpr float RAD_TO_INDEX = (float) SIN_COUNT / PI2;
constexpr float DEG_TO_INDEX = (float) SIN_COUNT / 360.0f;

static float* table = new float[SIN_COUNT];

void initialize_math() {
  for (int i = 0; i < SIN_COUNT; i++)
    table[i] = (float) sinf((i + 0.5f) / SIN_COUNT * PI2);

  table[0] = 0;
  table[(int) (90 * DEG_TO_INDEX) & SIN_MASK] = 1;
  table[(int) (180 * DEG_TO_INDEX) & SIN_MASK] = 0;
  table[(int) (270 * DEG_TO_INDEX) & SIN_MASK] = -1;
}

float sin_approximate(float radians) {
  return table[(int) (radians * RAD_TO_INDEX) & SIN_MASK];
}

float cos_approximate(float radians) {
  return table[(int) ((radians + M_PI_2) * RAD_TO_INDEX) & SIN_MASK];
}