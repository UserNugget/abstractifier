#version 330
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
uniform vec2 offset;
uniform float ratio;

out vec4 fragColor;

in vec2 viewResolution;
in vec2 viewOffset;
in vec2 ratios;

void main() {
    fragColor = vec4(vec3(0), 1.0);
}
