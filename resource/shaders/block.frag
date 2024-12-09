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
    vec2 uv = ((gl_FragCoord.xy - viewOffset) + ratios) / viewResolution;
    vec2 coords = (gl_FragCoord.xy - viewOffset + ratios);
    float temp = float(int(coords.x / 3.) ^ int(coords.y / 3.));

    fragColor = vec4((vec3(201, 16, 10) / 255.) + 0.1 * tan(temp), 1.0);
}
