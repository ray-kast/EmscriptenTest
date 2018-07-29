#pragma once

#include <vector>

#include <EGL/egl.h>

namespace cegl {
std::vector<EGLint> makeAttribList(const std::vector<EGLint> &);
}
