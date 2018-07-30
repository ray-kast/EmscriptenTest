#version 100

attribute vec3 in_POSITION;

uniform mat4 u_MAT_TRANSFORM;

void main() {
  gl_Position = u_MAT_TRANSFORM * vec4(in_POSITION, 1.0);
}
