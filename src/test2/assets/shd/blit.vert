#version 100

attribute vec3 in_POSITION;
attribute vec3 in_COLOR;
attribute vec2 in_UV0;

varying vec3 vf_COLOR;
varying vec2 vf_UV0;

uniform mat4 u_MAT_TRANSFORM;

void main() {
  gl_Position = u_MAT_TRANSFORM * vec4(in_POSITION, 1.0);
  vf_COLOR    = in_COLOR;
  vf_UV0      = in_UV0;
}
