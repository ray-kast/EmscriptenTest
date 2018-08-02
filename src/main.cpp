#include "program.hpp"

#if defined(_JS)
#include <emscripten.h>

#include <diag.hpp>

static double   s_startTime;
static Program *s_pgm;

inline double now() { return emscripten_get_now() / 1e3; }

static void mainLoop() { s_pgm->render(now() - s_startTime); }

int main(int argc, char **argv) {
  s_pgm = new Program(argc, argv);

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
  // NB: no commas allowed here
  DIAG_IF(bool traceRender = true; bool traceResize = true;)

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

        // TODO: deal with extra call to resize()
        DIAG_IF(bool trace = traceResize; if (trace) {
          info("[======== \e[1mresize start\e[0m ========]");
          traceResize = false;
        } else glDiagOff();)

        pgm.resize(evt.width, evt.height);

        DIAG_IF(if (trace) info("[========  \e[1mresize end\e[0m  ========]");
                else glDiagOn();)

        break;
      }
      case KeyPress: {
        auto &&evt = xev.xkey;

        char   chr;
        KeySym _;

        if (XLookupString(&evt, &chr, 1, &_, nullptr)) {
          switch (chr) {
          case '\x1b': goto stop;
          case 'q': goto stop;
          }
        }

        break;
      }
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

    DIAG_IF(bool trace = traceRender; if (trace) {
      info("[======== \e[1mframe start\e[0m ========]");
      traceRender = false;
    } else glDiagOff();)

    pgm.render(now() - startTime);

    DIAG_IF(if (trace) info("[========  \e[1mframe end\e[0m  ========]");
            else glDiagOn();)
  }

stop:
  return 0;
}
#endif
