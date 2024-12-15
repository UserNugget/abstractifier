#version 330

in vec2 position;
in vec4 color;
in vec2 texture;

out vec4 backgroundColor;

void main() {
  gl_Position = vec4(position, 0.4, 1.0);
  backgroundColor = color;
}