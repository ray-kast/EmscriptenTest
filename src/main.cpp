#include "program.hpp"

#if defined(_JS)
#include <emscripten.h>

#include <diag.hpp>

static double   s_startTime;
static Program *s_pgm;

inline double now() { return emscripten_get_now() / 1e3; }

static void mainLoop() {
  try {
    s_pgm->render(now() - s_startTime);
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

  s_startTime = now();

  emscripten_set_main_loop(mainLoop, 0, false);
}
#else
#include <time.h>

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

inline double now() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

  return double(ts.tv_sec) + double(ts.tv_nsec) / 1e9;
}

int main(int argc, char **argv) {
  XSetErrorHandler(handleXError);
  XSetIOErrorHandler(handleXIOError);

  Program pgm(argc, argv);

  auto &&disp = pgm.xDisp();

  Atom wmProtocols    = disp.internAtom("WM_PROTOCOLS");
  Atom wmDeleteWindow = disp.internAtom("WM_DELETE_WINDOW");

  double startTime = now();

  while (true) {
    while (disp.pending()) {
      auto xev = disp.nextEvent();

      switch (xev.type) {
      case ConfigureNotify: {
        auto &&evt = xev.xconfigure;

        pgm.resize(evt.width, evt.height);

        break;
      }
      case KeyPress: goto stop;
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

    pgm.render(now() - startTime);
  }

stop:
  return 0;
}
#endif
