#version 330

uniform vec2 resolution;
uniform vec2 mouse;
uniform vec2 offset;

in vec3 position;

out vec2 viewResolution;
out vec2 viewOffset;
out vec2 ratios;
out float properRatio;

void main() {
  float expectedWidth = 1280;
  float expectedHeight = 720;

  float ratioX = resolution.x / expectedWidth;
  float ratioY = resolution.y / expectedHeight;
  float ratio = ratioX < ratioY ? ratioX : ratioY;

  float viewWidth = expectedWidth * ratio;
  float viewHeight = expectedHeight * ratio;

  float viewX = ((resolution.x - expectedWidth * ratio) / 2);
  float viewY = ((resolution.y - expectedHeight * ratio) / 2);

  viewOffset = vec2(viewX, viewY);
  viewResolution = vec2(viewWidth, viewHeight);
  ratios = offset * ratio;
  properRatio = ratio;

  gl_Position = vec4(position, 1.0);
}