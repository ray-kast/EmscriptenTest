#include <iostream>

#include <Eigen/Eigen>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <matrixMath.hpp>

#include <cegl/context.hpp>
#include <cx/display.hpp>
#include <cx/window.hpp>

int main(int, char **) {
  Eigen::Vector3f vec(0.0f, 1.0f, 2.0f);

  cx::Display   disp(nullptr);
  auto          root = cx::Window::root(disp);
  cx::Window    win(root);
  cegl::Context ctx(win);

  return 0;
}
