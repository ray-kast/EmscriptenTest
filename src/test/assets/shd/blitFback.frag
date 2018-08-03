#version 100

precision mediump float;

varying vec3 vf_COLOR;
varying vec2 vf_UV0;

uniform sampler2D u_S2D_TEXTURE;
uniform float u_FLT_TIME;
uniform float u_FLT_DT;

void main() {
  vec2 uv11 = vf_UV0 * 2.0 - vec2(1.0);

  vec2 offs = uv11 * abs(uv11.y) + vec2(uv11.x, 0) * 0.1;

  offs *= -1.0;

  vec4 clr = texture2D(u_S2D_TEXTURE, vf_UV0 + offs * u_FLT_DT);

  clr *= vec4(vf_COLOR, 1.0);

  // clr = mix(clr, vec4(0.0, 0.0, 0.0, 1.0), clamp(3.0 * u_FLT_DT, 0.0, 1.0));

  gl_FragColor = clr;
}
