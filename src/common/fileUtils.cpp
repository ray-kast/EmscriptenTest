#include "fileUtils.hpp"

#include <fstream>
#include <stdexcept>

#if !defined(_JS)
#include <unistd.h>
#endif

std::string readFile(const std::string &file) {
  std::ifstream ifs(file);

  if (!ifs) throw std::runtime_error("failed to open '" + file + "'");

  return std::string(std::istreambuf_iterator<char>(ifs),
                     std::istreambuf_iterator<char>());
}

std::string getExeDir() {
#if defined(_JS)
  return "/";
#else
  char buf[1024];
  auto size = readlink("/proc/self/exe", buf, sizeof(buf));

  if (size < 0)
    throw std::runtime_error("readlink /proc/self/exe failed");
  else if (size >= sizeof(buf))
    throw std::runtime_error("readlink /proc/self/exe overflowed");

  buf[size] = 0;

  std::string exe(buf);
  auto        lastIdx = exe.find_last_of('/');

  if (lastIdx == std::string::npos) return exe;

  return std::string(exe, 0, lastIdx + 1);
#endif
}
