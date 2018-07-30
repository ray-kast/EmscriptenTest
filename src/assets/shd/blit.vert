#version 100

attribute vec3 in_POSITION;

void main() {
  gl_Position = vec4(in_POSITION, 1.0);
}
