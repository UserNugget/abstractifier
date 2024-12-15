#version 330

uniform float time;

in vec2 viewResolution;
in vec2 viewOffset;
in vec4 buttonColor;
out vec4 fragColor;

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float random(in vec2 _st) {
  return fract(sin(dot(_st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float fbm(in vec2 _st) {
  float v = 0.0;
  float a = 0.5;
  vec2 shift = vec2(100.0);
  mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
  for (int j = 0; j < 2; ++j) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    v += a * mix(a, b, u.x) +
      (c - a)* u.y * (1.0 - u.x) +
      (d - b) * u.x * u.y;
    _st = rot * _st * 2.0 + shift;
    a *= 0.5;
  }
  return v;
}    

void main() {
  vec2 uv = ((gl_FragCoord.xy - viewOffset + vec2(128, 0)) / viewResolution);
  vec3 col = 0.5 + 0.5 * tan(fbm(uv) + time / 8.0 + uv.xyx + vec3(0,2,4));
  fragColor = vec4(col, 1.0);
}