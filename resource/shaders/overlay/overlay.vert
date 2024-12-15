#version 330

in vec2 position;
in vec4 color;

out vec4 overlayColor;

void main() {
  gl_Position = vec4(position, 0, 1.0);
  overlayColor = color;
}
