#version 330

#define WIDTH 1280.0
#define HEIGHT 720.0
#define EXPECTED_RESOLUTION vec2(WIDTH, HEIGHT)

uniform vec2 resolution;
uniform vec2 mouse;
uniform vec2 offset;

in vec2 position;
in vec4 color;

out vec2 viewResolution;
out vec2 viewOffset;
out vec2 viewOffsetRatio;
out vec4 fragmentColor;

void main() {
  fragmentColor = color;

  float ratio = min(resolution.x / WIDTH, resolution.y / HEIGHT);
  viewResolution = EXPECTED_RESOLUTION * ratio;
  viewOffset = (resolution - viewResolution) / 2.0;
  viewOffsetRatio = offset * ratio;

  gl_Position = vec4(position, 0, 1);
}