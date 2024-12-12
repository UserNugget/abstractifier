#version 330

// https://www.shadertoy.com/view/Mt2GWD
uniform vec2 resolution;
uniform vec2 offset;
uniform float frameRate;
uniform float tickTime;
uniform float entityCount;
uniform float score;

in vec2 viewResolution;
in vec2 viewOffset;

out vec4 fragColor;

#define DOWN_SCALE 2.0

#define MAX_INT_DIGITS 6

#define CHAR_SIZE vec2(8, 12)
#define CHAR_SPACING vec2(8, 12)

#define STRWIDTH(c) (c * CHAR_SPACING.x)
#define STRHEIGHT(c) (c * CHAR_SPACING.y)

#define NORMAL 0
#define INVERT 1
#define UNDERLINE 2

int TEXT_MODE = INVERT;

const vec4 ch_spc = vec4(0x000000,0x000000,0x000000,0x000000);
const vec4 ch_exc = vec4(0x003078,0x787830,0x300030,0x300000);
const vec4 ch_quo = vec4(0x006666,0x662400,0x000000,0x000000);
const vec4 ch_hsh = vec4(0x006C6C,0xFE6C6C,0x6CFE6C,0x6C0000);
const vec4 ch_dol = vec4(0x30307C,0xC0C078,0x0C0CF8,0x303000);
const vec4 ch_pct = vec4(0x000000,0xC4CC18,0x3060CC,0x8C0000);
const vec4 ch_amp = vec4(0x0070D8,0xD870FA,0xDECCDC,0x760000);
const vec4 ch_apo = vec4(0x003030,0x306000,0x000000,0x000000);
const vec4 ch_lbr = vec4(0x000C18,0x306060,0x603018,0x0C0000);
const vec4 ch_rbr = vec4(0x006030,0x180C0C,0x0C1830,0x600000);
const vec4 ch_ast = vec4(0x000000,0x663CFF,0x3C6600,0x000000);
const vec4 ch_crs = vec4(0x000000,0x18187E,0x181800,0x000000);
const vec4 ch_com = vec4(0x000000,0x000000,0x000038,0x386000);
const vec4 ch_dsh = vec4(0x000000,0x0000FE,0x000000,0x000000);
const vec4 ch_per = vec4(0x000000,0x000000,0x000038,0x380000);
const vec4 ch_lsl = vec4(0x000002,0x060C18,0x3060C0,0x800000);
const vec4 ch_0 = vec4(0x007CC6,0xD6D6D6,0xD6D6C6,0x7C0000);
const vec4 ch_1 = vec4(0x001030,0xF03030,0x303030,0xFC0000);
const vec4 ch_2 = vec4(0x0078CC,0xCC0C18,0x3060CC,0xFC0000);
const vec4 ch_3 = vec4(0x0078CC,0x0C0C38,0x0C0CCC,0x780000);
const vec4 ch_4 = vec4(0x000C1C,0x3C6CCC,0xFE0C0C,0x1E0000);
const vec4 ch_5 = vec4(0x00FCC0,0xC0C0F8,0x0C0CCC,0x780000);
const vec4 ch_6 = vec4(0x003860,0xC0C0F8,0xCCCCCC,0x780000);
const vec4 ch_7 = vec4(0x00FEC6,0xC6060C,0x183030,0x300000);
const vec4 ch_8 = vec4(0x0078CC,0xCCEC78,0xDCCCCC,0x780000);
const vec4 ch_9 = vec4(0x0078CC,0xCCCC7C,0x181830,0x700000);
const vec4 ch_col = vec4(0x000000,0x383800,0x003838,0x000000);
const vec4 ch_scl = vec4(0x000000,0x383800,0x003838,0x183000);
const vec4 ch_les = vec4(0x000C18,0x3060C0,0x603018,0x0C0000);
const vec4 ch_equ = vec4(0x000000,0x007E00,0x7E0000,0x000000);
const vec4 ch_grt = vec4(0x006030,0x180C06,0x0C1830,0x600000);
const vec4 ch_que = vec4(0x0078CC,0x0C1830,0x300030,0x300000);
const vec4 ch_ats = vec4(0x007CC6,0xC6DEDE,0xDEC0C0,0x7C0000);
const vec4 ch_A = vec4(0x003078,0xCCCCCC,0xFCCCCC,0xCC0000);
const vec4 ch_B = vec4(0x00FC66,0x66667C,0x666666,0xFC0000);
const vec4 ch_C = vec4(0x003C66,0xC6C0C0,0xC0C666,0x3C0000);
const vec4 ch_D = vec4(0x00F86C,0x666666,0x66666C,0xF80000);
const vec4 ch_E = vec4(0x00FE62,0x60647C,0x646062,0xFE0000);
const vec4 ch_F = vec4(0x00FE66,0x62647C,0x646060,0xF00000);
const vec4 ch_G = vec4(0x003C66,0xC6C0C0,0xCEC666,0x3E0000);
const vec4 ch_H = vec4(0x00CCCC,0xCCCCFC,0xCCCCCC,0xCC0000);
const vec4 ch_I = vec4(0x007830,0x303030,0x303030,0x780000);
const vec4 ch_J = vec4(0x001E0C,0x0C0C0C,0xCCCCCC,0x780000);
const vec4 ch_K = vec4(0x00E666,0x6C6C78,0x6C6C66,0xE60000);
const vec4 ch_L = vec4(0x00F060,0x606060,0x626666,0xFE0000);
const vec4 ch_M = vec4(0x00C6EE,0xFEFED6,0xC6C6C6,0xC60000);
const vec4 ch_N = vec4(0x00C6C6,0xE6F6FE,0xDECEC6,0xC60000);
const vec4 ch_O = vec4(0x00386C,0xC6C6C6,0xC6C66C,0x380000);
const vec4 ch_P = vec4(0x00FC66,0x66667C,0x606060,0xF00000);
const vec4 ch_Q = vec4(0x00386C,0xC6C6C6,0xCEDE7C,0x0C1E00);
const vec4 ch_R = vec4(0x00FC66,0x66667C,0x6C6666,0xE60000);
const vec4 ch_S = vec4(0x0078CC,0xCCC070,0x18CCCC,0x780000);
const vec4 ch_T = vec4(0x00FCB4,0x303030,0x303030,0x780000);
const vec4 ch_U = vec4(0x00CCCC,0xCCCCCC,0xCCCCCC,0x780000);
const vec4 ch_V = vec4(0x00CCCC,0xCCCCCC,0xCCCC78,0x300000);
const vec4 ch_W = vec4(0x00C6C6,0xC6C6D6,0xD66C6C,0x6C0000);
const vec4 ch_X = vec4(0x00CCCC,0xCC7830,0x78CCCC,0xCC0000);
const vec4 ch_Y = vec4(0x00CCCC,0xCCCC78,0x303030,0x780000);
const vec4 ch_Z = vec4(0x00FECE,0x981830,0x6062C6,0xFE0000);
const vec4 ch_lsb = vec4(0x003C30,0x303030,0x303030,0x3C0000);
const vec4 ch_rsl = vec4(0x000080,0xC06030,0x180C06,0x020000);
const vec4 ch_rsb = vec4(0x003C0C,0x0C0C0C,0x0C0C0C,0x3C0000);
const vec4 ch_pow = vec4(0x10386C,0xC60000,0x000000,0x000000);
const vec4 ch_usc = vec4(0x000000,0x000000,0x000000,0x00FF00);
const vec4 ch_a = vec4(0x000000,0x00780C,0x7CCCCC,0x760000);
const vec4 ch_b = vec4(0x00E060,0x607C66,0x666666,0xDC0000);
const vec4 ch_c = vec4(0x000000,0x0078CC,0xC0C0CC,0x780000);
const vec4 ch_d = vec4(0x001C0C,0x0C7CCC,0xCCCCCC,0x760000);
const vec4 ch_e = vec4(0x000000,0x0078CC,0xFCC0CC,0x780000);
const vec4 ch_f = vec4(0x00386C,0x6060F8,0x606060,0xF00000);
const vec4 ch_g = vec4(0x000000,0x0076CC,0xCCCC7C,0x0CCC78);
const vec4 ch_h = vec4(0x00E060,0x606C76,0x666666,0xE60000);
const vec4 ch_i = vec4(0x001818,0x007818,0x181818,0x7E0000);
const vec4 ch_j = vec4(0x000C0C,0x003C0C,0x0C0C0C,0xCCCC78);
const vec4 ch_k = vec4(0x00E060,0x60666C,0x786C66,0xE60000);
const vec4 ch_l = vec4(0x007818,0x181818,0x181818,0x7E0000);
const vec4 ch_m = vec4(0x000000,0x00FCD6,0xD6D6D6,0xC60000);
const vec4 ch_n = vec4(0x000000,0x00F8CC,0xCCCCCC,0xCC0000);
const vec4 ch_o = vec4(0x000000,0x0078CC,0xCCCCCC,0x780000);
const vec4 ch_p = vec4(0x000000,0x00DC66,0x666666,0x7C60F0);
const vec4 ch_q = vec4(0x000000,0x0076CC,0xCCCCCC,0x7C0C1E);
const vec4 ch_r = vec4(0x000000,0x00EC6E,0x766060,0xF00000);
const vec4 ch_s = vec4(0x000000,0x0078CC,0x6018CC,0x780000);
const vec4 ch_t = vec4(0x000020,0x60FC60,0x60606C,0x380000);
const vec4 ch_u = vec4(0x000000,0x00CCCC,0xCCCCCC,0x760000);
const vec4 ch_v = vec4(0x000000,0x00CCCC,0xCCCC78,0x300000);
const vec4 ch_w = vec4(0x000000,0x00C6C6,0xD6D66C,0x6C0000);
const vec4 ch_x = vec4(0x000000,0x00C66C,0x38386C,0xC60000);
const vec4 ch_y = vec4(0x000000,0x006666,0x66663C,0x0C18F0);
const vec4 ch_z = vec4(0x000000,0x00FC8C,0x1860C4,0xFC0000);
const vec4 ch_lpa = vec4(0x001C30,0x3060C0,0x603030,0x1C0000);
const vec4 ch_bar = vec4(0x001818,0x181800,0x181818,0x180000);
const vec4 ch_rpa = vec4(0x00E030,0x30180C,0x183030,0xE00000);
const vec4 ch_tid = vec4(0x0073DA,0xCE0000,0x000000,0x000000);
const vec4 ch_lar = vec4(0x000000,0x10386C,0xC6C6FE,0x000000);

const vec4[] numbers = vec4[10](ch_0, ch_1, ch_2, ch_3, ch_4, ch_5, ch_6, ch_7, ch_8, ch_9);
const float[] powers = float[MAX_INT_DIGITS + 1](1, 10, 100, 1000, 10000, 100000, 1000000);

vec2 res = vec2(0);
vec2 print_pos = vec2(0);
float maxSize = 0;

float extract_bit(float n, float b) {
    b = clamp(b,-1.0,24.0);
	return floor(mod(floor(n / pow(2.0,floor(b))),2.0));   
}

float sprite(vec4 spr, vec2 size, vec2 uv) {
    uv = floor(uv);
    if (!all(greaterThanEqual(uv, vec2(0))) || !all(lessThan(uv, size))) {
      return 0.0;
    }
    
    float pixels = 0.0;
    float bit = (size.x - uv.x - 1.0) + uv.y * size.x;
    pixels += extract_bit(spr.x, bit - 72.0);
    pixels += extract_bit(spr.y, bit - 48.0);
    pixels += extract_bit(spr.z, bit - 24.0);
    pixels += extract_bit(spr.w, bit - 00.0);
    
    return pixels;
}

float char(vec4 ch, vec2 uv) {
    if (TEXT_MODE == INVERT) {
      ch = pow(2.0,24.0)-1.0-ch;
    }

    if (TEXT_MODE == UNDERLINE) {
      ch.w = floor(ch.w / 256.0) * 256.0 + 255.0;
    }

    float px = sprite(ch, CHAR_SIZE, uv - print_pos);
    print_pos.x += CHAR_SPACING.x;
    return px;
}

float print_integer(float number, vec2 uv) {
    float result = 0.0;
	for (int i = MAX_INT_DIGITS; i >= 0; i--) {
        float segment = powers[i];
        if (abs(number) >= segment || i <= 0) {
        	result += char(numbers[int(mod(number / segment, 10.0))], uv);
        }
    }
    return result;
}

float hud(vec2 uv) {
    // FPS: <fps>
    print_pos = floor(vec2(0., res.y - STRHEIGHT(1.0)));

    float col = 0.0;
    TEXT_MODE = INVERT;
    col += char(ch_F, uv);
    col += char(ch_P, uv);
    col += char(ch_S, uv);

    TEXT_MODE = NORMAL;
    col += char(ch_col, uv);

    TEXT_MODE = UNDERLINE;
    col += print_integer(frameRate, uv);

    // MSPT: <mspt>
    maxSize = max(maxSize, print_pos.x);
    print_pos = floor(vec2(0., res.y - STRHEIGHT(2.0)));

    TEXT_MODE = INVERT;
    col += char(ch_M, uv);
    col += char(ch_S, uv);
    col += char(ch_P, uv);
    col += char(ch_T, uv);

    TEXT_MODE = NORMAL;
    col += char(ch_col, uv);

    TEXT_MODE = UNDERLINE;
    col += print_integer(tickTime, uv);

    // Objects: <objects>
    maxSize = max(maxSize, print_pos.x);
    print_pos = floor(vec2(0., res.y - STRHEIGHT(3.0)));

    TEXT_MODE = INVERT;
    col += char(ch_O, uv);
    col += char(ch_b, uv);
    col += char(ch_j, uv);
    col += char(ch_e, uv);
    col += char(ch_c, uv);
    col += char(ch_t, uv);
    col += char(ch_s, uv);

    TEXT_MODE = NORMAL;
    col += char(ch_col, uv);

    TEXT_MODE = UNDERLINE;
    col += print_integer(entityCount, uv);

    // Score: <score>
    maxSize = max(maxSize, print_pos.x);
    print_pos = floor(vec2(0., res.y - STRHEIGHT(4.0)));

    TEXT_MODE = INVERT;
    col += char(ch_S, uv);
    col += char(ch_c, uv);
    col += char(ch_o, uv);
    col += char(ch_r, uv);
    col += char(ch_e, uv);

    TEXT_MODE = NORMAL;
    col += char(ch_col, uv);

    TEXT_MODE = UNDERLINE;
    col += print_integer(score, uv);

    maxSize = max(maxSize, print_pos.x);
    return col;
}

void main() {
    vec2 coords = gl_FragCoord.xy - viewOffset;
    vec2 uv = coords / DOWN_SCALE;
    res = viewResolution.xy / DOWN_SCALE;
    if (uv.x > 200 || res.y - uv.y > STRHEIGHT(4.0)) {
      discard;
    }

    maxSize = 0;
	float pixel = hud(floor(coords / DOWN_SCALE));
	if (uv.x > maxSize) {
	  discard;
	}

	fragColor = vec4(mix(vec3(0.2), vec3(0,1,0), pixel), 1.0);
}
