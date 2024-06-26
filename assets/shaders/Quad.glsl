@type vertex

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in uint aTexIndex;
layout (location = 4) in uint aEffectMode;
layout (location = 5) in vec2 aQuadSize;

out vec4 vColor;
out vec2 vTexCoord;
flat out uint vTexIndex;
flat out uint vEffectMode;
flat out vec2 vQuadSize;

void main() {
   vColor      = aColor;
   vTexCoord   = aTexCoord;
   vTexIndex   = aTexIndex;
   vEffectMode = aEffectMode;
   vQuadSize   = aQuadSize;
   gl_Position = vec4(aPosition, 0.0f, 1.0f);
}

@type fragment

precision mediump float;

out vec4 vFragColor;

in vec4 vColor;
in vec2 vTexCoord;
flat in uint vTexIndex;
flat in uint vEffectMode;
flat in vec2 vQuadSize;

uniform sampler2D uTextures[16];

uniform float uTime;
uniform vec2  uResolution;

vec4 getTextureColor() {
  // Reason for the switch cases: 
  // https://stackoverflow.com/questions/72648980/opengl-sampler2d-array
  vec4 color;
  switch (vTexIndex) {
    case  0u: color = texture(uTextures[ 0], vTexCoord); break;
    case  1u: color = texture(uTextures[ 1], vTexCoord); break;
    case  2u: color = texture(uTextures[ 2], vTexCoord); break;
    case  3u: color = texture(uTextures[ 3], vTexCoord); break;
    case  4u: color = texture(uTextures[ 4], vTexCoord); break;
    case  5u: color = texture(uTextures[ 5], vTexCoord); break;
    case  6u: color = texture(uTextures[ 6], vTexCoord); break;
    case  7u: color = texture(uTextures[ 7], vTexCoord); break;
    case  8u: color = texture(uTextures[ 8], vTexCoord); break;
    case  9u: color = texture(uTextures[ 9], vTexCoord); break;
    case 10u: color = texture(uTextures[10], vTexCoord); break;
    case 11u: color = texture(uTextures[11], vTexCoord); break;
    case 12u: color = texture(uTextures[12], vTexCoord); break;
    case 13u: color = texture(uTextures[13], vTexCoord); break;
    case 14u: color = texture(uTextures[14], vTexCoord); break;
    case 15u: color = texture(uTextures[15], vTexCoord); break;
  }
  return color;
}

void effect_striped(out vec4 fragColor) {
  vec2 frag_uv = gl_FragCoord.xy / uResolution;
  vec3 color_gap = vec3(0.25);
  // vec3 color_stripe = vec3(1.0, 0.75, 0.0);
  // vec3 color_stripe = vec3(1.0, 0.20, 0.0);
  vec3 color_stripe = vColor.rgb;
  float divisions = 12.0; // increase for more stripe density
  float stripe_bias = 2.0; // 1.0 means no stripes; 2.0 means stripes and gaps are equal size
  float speed = 0.1;
  float angle = 0.7854; // in radians
  float w = cos(angle) * frag_uv.x + sin(angle) * frag_uv.y - speed * uTime;
  if (floor(mod(w * divisions, stripe_bias)) < 0.0001) {
    fragColor = vec4(color_gap, 1.0f);
  } else {
    fragColor = vec4(color_stripe, 1.0f);
  }
}


float Noise21(vec2 p, float ta, float tb) {
   return fract(sin(p.x * ta + p.y * tb) * 5678.0f);
}

void effect_staticNoise(out vec4 fragColor) {
  vec2 uv = gl_FragCoord.xy / uResolution.xy;

  float t = uTime + 123.0f; // tweak the start moment
  float ta = t * 0.654321f;
  float tb = t * (ta * 0.123456f);
  
  float c = Noise21(uv, ta, tb);

  vec3 color = vec3(c);
  fragColor = vec4(color, 1.0f);
}


float RectSDF(vec2 p, vec2 b, float r) {
   vec2 d = abs(p) - b + vec2(r);
   return min(max(d.x, d.y), 0.0f) + length(max(d, 0.0f)) - r;
}

void effect_roundedCorners(out vec4 fragColor) {
   const float borderThickness = 3.0f;
   const float radius = 12.0f;
   vec4  borderColor = vColor;

   vec2 pos = vQuadSize * vTexCoord;
        
   float dist = RectSDF(pos - vQuadSize / 2.0f, vQuadSize / 2.0f - borderThickness / 2.0f - 1.0f, radius);
   float blendAmount = smoothstep(-1.0f, 1.0f, abs(dist) - borderThickness / 2.0f);

   vec4 fromColor = borderColor;
   vec4 toColor = (dist < 0.0f) ? getTextureColor() : vec4(0.0f);
   fragColor = mix(fromColor, toColor, blendAmount);
}

void main() {
   vFragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
   switch (vEffectMode) {
      case 0u: {
         vFragColor = vColor * getTextureColor();
      } break;
      case 1u: {
         effect_striped(vFragColor);
      } break;
      case 2u: {
        effect_staticNoise(vFragColor);
      } break;
      case 3u: {
        effect_roundedCorners(vFragColor);
      } break;
   }
}
