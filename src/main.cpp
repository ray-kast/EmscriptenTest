#include "program.hpp"

#if defined(_JS)
#include <emscripten.h>

static Program *s_program;

static void mainLoop() { s_program->mainLoop(); }

int main(int argc, char **argv) {
  s_program = new Program(argc, argv);

  emscripten_set_main_loop(mainLoop, 30, false);
}
#else
#include <GLES2/gl2.h>

#include <diag.hpp>

int main(int argc, char **argv) {
  Program program(argc, argv);

  while (program.mainLoop())
    ;

  return 0;
}
#endif
