#include "models.hpp"

#include <array>

namespace mdl {
void blitQuad(cgl::Model &           model,
              float                  z,
              const Eigen::Vector3f &color,
              cgl::DataFreq          freq,
              cgl::DataAccess        access) {
  float           pos[4][3]{{-1, -1, z}, {1, -1, z}, {-1, 1, z}, {1, 1, z}};
  Eigen::Vector3f clr[4]{color, color, color, color};
  float           uv0[4][2]{{0, 0}, {1, 0}, {0, 1}, {1, 1}};
  unsigned short  idx[2][3]{{0, 1, 3}, {3, 2, 0}};

  model.bindVbuf(0).data(pos, freq, access);
  model.bindVbuf(1).data(clr, freq, access);
  model.bindVbuf(2).data(uv0, freq, access);

  model.bindIbuf().data(idx, freq, access);
  model.ibufLen(sizeof(idx) / sizeof(**idx));
}

void strokePath(cgl::Model &           model,
                const pth::Path &      path,
                int                    res,
                float                  w,
                const Eigen::Vector3f &color,
                cgl::DataFreq          freq,
                cgl::DataAccess        access) {
  auto [pos, idx] = pth::stroke(path, res, w);

  std::vector<Eigen::Vector3f> clr(pos.size());
  std::vector<Eigen::Vector2f> uv0(pos.size());

  for (std::size_t i = 0; i < pos.size(); ++i) {
    clr.emplace(clr.begin() + i, color);
    uv0.emplace(uv0.begin() + i,
                pos[i].template head<2>()); // TODO: this needs work
    // uv0[i].setZero();
  }

  model.bindVbuf(0).data(pos, freq, access);
  model.bindVbuf(1).data(clr, freq, access);
  model.bindVbuf(2).data(uv0, freq, access);

  model.bindIbuf().data(idx, freq, access);
  model.ibufLen(idx.size() * 3);
}

void fanPath(cgl::Model &           model,
             const pth::Path &      path,
             int                    res,
             const Eigen::Vector3f &color,
             cgl::DataFreq          freq,
             cgl::DataAccess        access) {
  auto [pos, idx] = pth::fan(path, res);

  std::vector<Eigen::Vector3f> clr(pos.size());
  std::vector<Eigen::Vector2f> uv0(pos.size());

  for (std::size_t i = 0; i < pos.size(); ++i) {
    clr.emplace(clr.begin() + i, color);
    uv0.emplace(uv0.begin() + i,
                pos[i].template head<2>()); // TODO: this needs work
    // uv0[i].setZero();
  }

  model.bindVbuf(0).data(pos, freq, access);
  model.bindVbuf(1).data(clr, freq, access);
  model.bindVbuf(2).data(uv0, freq, access);

  model.bindIbuf().data(idx, freq, access);
  model.ibufLen(idx.size() * 3);
}
} // namespace mdl
