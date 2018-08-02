#pragma once

#include <random>

#include <cx/display.hpp>
#include <cx/window.hpp>

#include <cegl/context.hpp>
#include <cegl/display.hpp>
#include <cegl/makeCurrent.hpp>
#include <cegl/surface.hpp>

#include <cgles2/framebuffers.hpp>
#include <cgles2/material.hpp>
#include <cgles2/model.hpp>
#include <cgles2/textureUnits.hpp>

struct Particle {
  Eigen::Vector2f pos;
  Eigen::Vector3f clr;
  float           size, life, remain = 0.0f;
};

class Program {
  std::mt19937_64 m_mt;

  cx::Display m_xDisp;
  cx::Window  m_win;

  cegl::Display     m_disp;
  cegl::Surface     m_surf;
  cegl::Context     m_ctx;
  cegl::MakeCurrent m_makeCurrent;

  cgl::Material m_blit, m_particle;

  cgl::Model m_blitQuad, m_bkgdQuad, m_circle, m_fieldLines;

  cgl::TextureUnits m_white, m_mainFbufMap;

  cgl::Framebuffers m_mainFbufs;

  Eigen::Projective3f m_proj, m_view;

  std::vector<Particle> m_particles;
  std::size_t           m_nextParticle = 0;

  double m_lastTime = 0.0, m_spawnTime = 0.0;

  unsigned int m_width, m_height;

  void renderFieldLines();

  void renderParticles(double time, double dt);

public:
  constexpr auto &xDisp() { return m_xDisp; }

  Program(int, char **);

  ~Program();

  void render(double time);

  void resize(int w, int h);
};
