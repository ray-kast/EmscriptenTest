#include <iostream>

#include <Eigen/Eigen>

#include <GLES2/gl2.h>

int main(int, char **) {
  Eigen::Vector3f vec(0.0f, 1.0f, 2.0f);

  std::cout << "Hello world!" << std::endl;

  std::cout << vec * 2.0f << std::endl;

  return 0;
}
