#version 330

in vec2 position;
in vec4 color;
in vec2 texture;

out vec2 fontPosition;
out vec4 fontColor;

void main() {
  gl_Position = vec4(position, 0.5, 1.0);
  fontPosition = texture;
  fontColor = color;
}