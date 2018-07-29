#include "window.hpp"

#include <X11/Xutil.h>

#include "display.hpp"

namespace cx {
Window Window::root(const Display &disp) {
  return Window(disp, DefaultRootWindow(disp.m_disp.get()));
}

Window::Window(const Window &parent,
               int           x,
               int           y,
               unsigned int  width,
               unsigned int  height) :
    m_disp(parent.m_disp) {
  XSetWindowAttributes swa;

  swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
  swa.override_redirect =
      false; // TODO: if this is wrong, I don't want to be right

  m_win = XCreateWindow(m_disp->m_disp,
                        parent.m_win,
                        x,
                        y,
                        width,
                        height,
                        0,
                        CopyFromParent,
                        InputOutput,
                        CopyFromParent,
                        CWEventMask,
                        &swa);

  XWMHints hints;

  hints.input = true;
  hints.flags = InputHint;

  XSetWMHints(m_disp->m_disp, m_win, &hints);

  XMapWindow(m_disp->m_disp, m_win);
}
} // namespace cx
