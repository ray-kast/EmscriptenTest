#include "diag.hpp"

#include <iostream>
#include <stdexcept>

#if defined(_JS)
#define DIAG_STREAM std::cout
#else
#define DIAG_STREAM std::cerr
#endif

void info(const std::string &msg) {
#if defined(_JS)
  std::cout << "info: " << msg << std::endl;
#else
  std::cerr << "\e[1;38;5;8minfo:\e[0m " << msg << std::endl;
#endif
}

void warn(const std::string &msg) {
#if defined(_JS)
  std::cout << "warning: " << msg << std::endl;
#else
  std::cerr << "\e[1;38;5;5mwarning:\e[0m " << msg << std::endl;
#endif
}

void err(const std::string &msg) {
#if defined(_JS)
  std::cout << "error: " << msg << std::endl;
#else
  std::cerr << "\e[1;38;5;1merror:\e[0m " << msg << std::endl;
#endif
}

void die(const std::string &msg) {
  err(msg);
  throw std::runtime_error(msg);
}
