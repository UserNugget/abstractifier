#version 330
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
uniform vec2 offset;

in vec2 viewResolution;
in vec2 viewOffset;
in vec2 viewOffsetRatio;

out vec4 fragColor;

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    if (abs(mouse.x - gl_FragCoord.x) >= 12 || abs(mouse.y - gl_FragCoord.y) >= 12) {
        discard;
    }

    vec2 coords = (gl_FragCoord.xy - viewOffset + offset);
    float temp = float(int(coords.x / 3.) ^ int(coords.y / 3.));
    fragColor = vec4(hsv2rgb(vec3(1., 1., 1.) + sin(temp)), 1.0);
}