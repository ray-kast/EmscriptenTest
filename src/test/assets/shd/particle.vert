#version 100

attribute vec3 in_POSITION;

uniform mat4 u_MAT_WORLD;
uniform mat4 u_MAT_VIEW;
uniform mat4 u_MAT_PROJ;

void main() {
  vec4 worldPos = u_MAT_WORLD * vec4(in_POSITION, 1.0),
       viewPos  = u_MAT_VIEW * worldPos;

  gl_Position = u_MAT_PROJ * viewPos;
}