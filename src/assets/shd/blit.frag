#version 100

precision mediump float;

varying in vec3 vf_COLOR;
varying in vec2 vf_UV0;

uniform sampler2D u_S2D_TEXTURE;

void main() {
  gl_FragColor = texture2D(u_S2D_TEXTURE, vf_UV0) * vec4(vf_COLOR, 1.0);
}
