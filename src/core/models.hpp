#pragma once

#include <Eigen/Eigen>

#include "cgles2/model.hpp"

namespace mdl {
void blitQuad(cgl::Model &, float z, const Eigen::Vector3f &clr);
}
