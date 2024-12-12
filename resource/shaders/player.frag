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
in float properRatio;

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 uv = ((gl_FragCoord.xy - viewOffset) + ratios) / viewResolution;
    vec2 coords = (gl_FragCoord.xy - viewOffset + ratios);
    float temp = float(int(coords.x / 3.) ^ int(coords.y / 3.));

    if (abs(mouse.x - gl_FragCoord.x) < 8 && abs(mouse.y - gl_FragCoord.y) < 8) {
        fragColor = vec4(hsv2rgb(vec3(1.) - tan(temp)), 1.0);
        return;
    }

    fragColor = vec4((vec3(9, 12, 217) / 255.) + tan(temp), 1.0);
}
