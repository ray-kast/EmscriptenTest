#include "program.hpp"

#if defined(_JS)
#include <emscripten.h>

#include <diag.hpp>

static Program *s_pgm;

static void mainLoop() {
  try {
    s_pgm->render();
  }
  catch (std::runtime_error &e) {
    err(e.what());
    throw;
  }
}

int main(int argc, char **argv) {
  try {
    s_pgm = new Program(argc, argv);
  }
  catch (std::runtime_error &e) {
    err(e.what());
    throw;
  }

  emscripten_set_main_loop(mainLoop, 0, false);
}
#else
#include <diag.hpp>

static int handleXError(Display *disp, XErrorEvent *evt) {
  char buf[512];
  XGetErrorText(disp, evt->error_code, buf, sizeof(buf) / sizeof(char));

  err("X error occurred: " + std::string(buf));

  return 0;
}

[[noreturn]] static int handleXIOError(Display *) {
  // TODO: can I get any more information about the error here?
  die("fatal XIO error occurred");
}

int main(int argc, char **argv) {
  XSetErrorHandler(handleXError);
  XSetIOErrorHandler(handleXIOError);

  Program pgm(argc, argv);

  auto &&disp = pgm.xDisp();

  Atom wmProtocols    = disp.internAtom("WM_PROTOCOLS");
  Atom wmDeleteWindow = disp.internAtom("WM_DELETE_WINDOW");

  while (true) {
    while (disp.pending()) {
      auto xev = disp.nextEvent();

      switch (xev.type) {
      case ConfigureNotify: {
        auto &&evt = xev.xconfigure;

        pgm.resize(evt.width, evt.height);

        break;
      }
      case KeyPress: break;
      case ClientMessage: {
        auto &&evt = xev.xclient;

        if (evt.message_type == wmProtocols) {
          auto &&msg = evt.data.l[0];

          if (msg == wmDeleteWindow)
            goto stop;
          else
            warn("unknown client message " + disp.atomName(msg));
        }

        break;
      }
      }
    }

    pgm.render();
  }

stop:
  return 0;
}
#endif
