#version 100

precision mediump float;

uniform vec4 u_VEC_COLOR;

void main() {
  gl_FragColor = u_VEC_COLOR;

  gl_FragColor /= pow(gl_FragColor.w, 1.0 / 1.5); // TODO: this is real dumb
}