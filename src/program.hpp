#pragma once

#include <cx/display.hpp>
#include <cx/window.hpp>

#include <cegl/context.hpp>
#include <cegl/display.hpp>
#include <cegl/makeCurrent.hpp>
#include <cegl/surface.hpp>

#include <cgles2/buffers.hpp>
#include <cgles2/material.hpp>
#include <cgles2/model.hpp>

class Program {
  cx::Display m_xDisp;
  cx::Window  m_win;

  cegl::Display     m_disp;
  cegl::Surface     m_surf;
  cegl::Context     m_ctx;
  cegl::MakeCurrent m_makeCurrent;

  cgl::Material m_blit;

  cgl::Model m_triangle;

public:
  Program(int, char **);

  ~Program();

  bool mainLoop();
};
