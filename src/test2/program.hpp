#pragma once

#include <cx/display.hpp>
#include <cx/window.hpp>

#include <cegl/context.hpp>
#include <cegl/display.hpp>
#include <cegl/makeCurrent.hpp>
#include <cegl/surface.hpp>

#include <cgles2/material.hpp>
#include <cgles2/model.hpp>
#include <cgles2/textureUnits.hpp>

class Program {
  cx::Display m_xDisp;
  cx::Window  m_win;

  cegl::Display     m_disp;
  cegl::Surface     m_surf;
  cegl::Context     m_ctx;
  cegl::MakeCurrent m_makeCurrent;

  cgl::Material m_blit;

  cgl::Model m_bkgdQuad, m_worm;

  cgl::TextureUnits m_white, m_wood, m_concrete;

  unsigned int m_width, m_height;

public:
  constexpr auto &xDisp() { return m_xDisp; }

  constexpr auto &width() const { return m_width; }
  constexpr auto &height() const { return m_height; }

  Program(int, char **);

  ~Program();

  void render(double time);

  void resize(int w, int h);
};
