#include "diag.hpp"

#include <iostream>

void info(const std::string &msg) {
  std::cerr << "\e[1;38;5;8minfo:\e[0m " << msg << std::endl;
}

void warn(const std::string &msg) {
  std::cerr << "\e[1;38;5;5mwarning:\e[0m " << msg << std::endl;
}

void err(const std::string &msg) {
  std::cerr << "\e[1;38;5;1merror:\e[0m " << msg << std::endl;
}

void die(const std::string &msg) {
  err(msg);
  abort();
}
