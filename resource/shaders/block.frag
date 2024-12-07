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

    float amplitude = 1.;
    float frequency = 1.;
    float x = time;
    float y = sin(x * frequency);
    float t = 0.01*(-time*130.0);
    y += sin(x*frequency*2.1 + t)*4.5;
    y += sin(x*frequency*1.72 + t*1.121)*4.0;
    y += sin(x*frequency*2.221 + t*0.437)*5.0;
    y += sin(x*frequency*3.1122+ t*4.269)*2.5;
    y *= amplitude*0.06;

    vec3 color = 0.5 + 0.5*tan(y+time+uv.xyx+vec3(0,2,4));
    fragColor = vec4(color * (vec3(1.) - tan(temp)), 1.0);
}
