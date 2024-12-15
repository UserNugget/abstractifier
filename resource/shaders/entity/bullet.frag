#version 330

uniform vec2 offset;

out vec4 fragColor;

in vec2 viewResolution;
in vec2 viewOffset;

void main() {
    fragColor = vec4(vec3(0), 1.0);
}
