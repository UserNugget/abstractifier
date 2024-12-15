#version 330

in vec2 viewResolution;
in vec2 viewOffset;
in vec2 viewOffsetRatio;
in vec4 fragmentColor;

out vec4 fragColor;

void main() {
    vec2 coords = gl_FragCoord.xy - viewOffset + viewOffsetRatio;
    float temp = float(int(coords.x / 3.) ^ int(coords.y / 3.));
    fragColor = vec4(vec3(fragmentColor.rgb) + 0.2 * tan(temp), 1.0);
}
