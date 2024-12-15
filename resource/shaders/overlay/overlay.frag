#version 330

uniform float progress;
uniform vec2 resolution;
uniform float time;

in vec4 overlayColor;
out vec4 fragColor;

float interpolate(float progress) {
  float interpolated = progress * progress * (3.0 - 2.0 * progress);
  return interpolated * interpolated * (3.0 - 2.0 * interpolated);
}

void main() {
  vec2 uv = gl_FragCoord.xy / resolution / 2.0;
  if (progress > 1.0) {
    fragColor = mix(overlayColor, vec4(0.0), step(uv.x + uv.y, 1.0 - interpolate(2.0 - progress)));
  } else {
    fragColor = mix(vec4(0.0), overlayColor, step(uv.x + uv.y, interpolate(progress)));
  }
}