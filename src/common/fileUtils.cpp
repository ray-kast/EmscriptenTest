#include "fileUtils.hpp"

#include <fstream>

#if !defined(_JS)
#include <unistd.h>
#endif

#include "diag.hpp"

std::string readFile(const std::string &file) {
  std::ifstream ifs(file);

  if (!ifs) die("failed to open '" + file + "'");

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
    die("readlink /proc/self/exe failed");
  else if (size >= sizeof(buf))
    die("readlink /proc/self/exe overflowed");

  buf[size] = 0;

  std::string exe(buf);
  auto        lastIdx = exe.find_last_of('/');

  if (lastIdx == std::string::npos) return exe;

  return std::string(exe, 0, lastIdx + 1);
#endif
}
