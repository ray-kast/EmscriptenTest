#include "program.hpp"

#if defined(_JS)
#include <emscripten.h>

#include <diag.hpp>

static Program *s_program;

static void mainLoop() {
  try {
    s_program->render();
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

  emscripten_set_main_loop(mainLoop, 0, false);
}
#else
int main(int argc, char **argv) {
  Program program(argc, argv);

  while (true) program.render();

  return 0;
}
#endif
