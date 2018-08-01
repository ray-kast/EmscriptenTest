#pragma once

#include <Eigen/Eigen>

#include <path.hpp>

#include "cgles2/model.hpp"

namespace mdl {
// TODO: this may need phasing out
inline void setup_basic4(cgl::Model &model) {
  model = cgl::Model(GL_TRIANGLES, 4);

  model.addVbuf(0, "in_POSITION", 3, GL_FLOAT, 0, nullptr);
  model.addVbuf(1, "in_COLOR", 3, GL_FLOAT, 0, nullptr);
  model.addVbuf(2, "in_UV0", 2, GL_FLOAT, 0, nullptr);
  model.addIbuf(3, GL_UNSIGNED_SHORT, nullptr);
}

inline void setupBlitQuad(cgl::Model &model) { setup_basic4(model); }

void blitQuad(cgl::Model &,
              float                  z,
              const Eigen::Vector3f &clr,
              cgl::DataFreq,
              cgl::DataAccess = cgl::AccessDraw);

inline void setupStrokePath(cgl::Model &model) { setup_basic4(model); }

void strokePath(cgl::Model &,
                const pth::Path &,
                int                    res,
                float                  w,
                const Eigen::Vector3f &clr,
                cgl::DataFreq,
                cgl::DataAccess = cgl::AccessDraw);

inline void setupFanPath(cgl::Model &model) { setup_basic4(model); }

void fanPath(cgl::Model &,
             const pth::Path &,
             int                    res,
             const Eigen::Vector3f &clr,
             cgl::DataFreq,
             cgl::DataAccess = cgl::AccessDraw);
} // namespace mdl
