#version 330

uniform sampler2D fontTexture;

in vec2 fontPosition;
in vec4 fontColor;
out vec4 fragColor;

void main() {
  fragColor = fontColor * texture(fontTexture, fontPosition).r;
}