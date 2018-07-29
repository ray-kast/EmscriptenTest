#include "attribList.hpp"

namespace cegl {
std::vector<EGLint> makeAttribList(const std::vector<EGLint> &attribs) {
  std::vector<EGLint> ret(attribs.size() + 1);
  ret.insert(ret.begin(), attribs.begin(), attribs.end());
  ret[attribs.size()] = EGL_NONE;
  return ret;
}
} // namespace cegl
