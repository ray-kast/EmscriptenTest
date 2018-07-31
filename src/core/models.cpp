#include "models.hpp"

namespace mdl {
void blitQuad(cgl::Model &model, float z, const Eigen::Vector3f &color) {
  model = cgl::Model(GL_TRIANGLES, 4);

  float           pos[4][3]{{-1, -1, z}, {1, -1, z}, {-1, 1, z}, {1, 1, z}};
  Eigen::Vector3f clr[4]{color, color, color, color};
  float           uv0[4][2]{{0, 0}, {1, 0}, {0, 1}, {1, 1}};
  unsigned short  idx[2][3]{{0, 1, 3}, {3, 2, 0}};

  cgl::BindBuffer(GL_ARRAY_BUFFER, model.addVbuf(0, 0, 3, GL_FLOAT, 0, nullptr))
      .data(pos, cgl::FreqStatic);
  cgl::BindBuffer(GL_ARRAY_BUFFER, model.addVbuf(1, 1, 3, GL_FLOAT, 0, nullptr))
      .data(clr, cgl::FreqStatic);
  cgl::BindBuffer(GL_ARRAY_BUFFER, model.addVbuf(2, 2, 2, GL_FLOAT, 0, nullptr))
      .data(uv0, cgl::FreqStatic);

  cgl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                  model.addIbuf(3, GL_UNSIGNED_SHORT, nullptr))
      .data(idx, cgl::FreqStatic);

  model.ibufLen(sizeof(idx) / sizeof(**idx));
}
} // namespace mdl
