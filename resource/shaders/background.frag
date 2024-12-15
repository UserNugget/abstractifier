#version 330

uniform vec2 offset;

in vec2 viewResolution;
in vec2 viewOffsetRatio;
in vec2 viewOffset;
in vec2 ratios;

out vec4 fragColor;

void main() {
    vec2 coords = gl_FragCoord.xy - viewOffset + viewOffsetRatio;
    float temp = float(int(coords.x / 3.) ^ int(coords.y / 3.));
    fragColor = vec4((vec3(200, 40, 224) / 255.) + (vec3(1.) - tan(temp)), 1.0);
}