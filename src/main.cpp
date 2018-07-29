#include "program.hpp"

#if defined(_JS)
#include <emscripten.h>

#include <diag.hpp>

static Program *s_program;

static void mainLoop() {
  try {
    s_program->mainLoop();
  }
  catch (std::runtime_error &e) {
    err(e.what());
    throw;
  }
}

int main(int argc, char **argv) {
  try {
    s_program = new Program(argc, argv);
  }
  catch (std::runtime_error &e) {
    err(e.what());
    throw;
  }

  emscripten_set_main_loop(mainLoop, 30, false);
}
#else
int main(int argc, char **argv) {
  Program program(argc, argv);

  while (program.mainLoop())
    ;

  return 0;
}
#endif
