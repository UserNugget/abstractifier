#version 330

uniform vec2 resolution;

in vec2 position;
in vec4 color;

out vec2 viewResolution;
out vec2 viewOffset;
out vec4 buttonColor;

void main() {
  float ratio = min(resolution.x / 1280.0, resolution.y / 720.0);
  viewOffset = vec2(((resolution.x - 1280 * ratio) / 2.0), ((resolution.y - 720.0 * ratio) / 2.0));
  viewResolution = vec2(1280.0, 720.0) * ratio;

  gl_Position = vec4(position, 0, 1);
  buttonColor = color;
}