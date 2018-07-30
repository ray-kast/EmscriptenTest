#include "configInfo.hpp"

#include <sstream>

#include <diag.hpp>

namespace cegl {
EGLint ConfigInfo::attr(EGLint id) const {
  EGLint ret;
  if (!eglGetConfigAttrib(m_disp->m_disp, m_config, id, &ret))
    die("eglGetConfigAttrib failed");
  return ret;
}
} // namespace cegl

namespace std {
string to_string(const cegl::ConfigInfo &inf, bool extra) {
  ostringstream oss;

  oss << "#" << inf.id();

  switch (inf.caveat()) {
  case EGL_SLOW_CONFIG: oss << " SLOW"; break;
  case EGL_NON_CONFORMANT_CONFIG: oss << " NON-CONFORMANT"; break;
  }

  oss << ": ";

  if (auto st = inf.surfType(); st) {
    if (st & EGL_PBUFFER_BIT) oss << "pbuf ";
    if (st & EGL_PIXMAP_BIT) oss << "pmap ";
    if (st & EGL_WINDOW_BIT) oss << "win ";
    if (st & EGL_MULTISAMPLE_RESOLVE_BOX_BIT) oss << "msbox ";
    if (st & EGL_SWAP_BEHAVIOR_PRESERVED_BIT) oss << "sbp ";
    if (st & EGL_VG_ALPHA_FORMAT_PRE_BIT) oss << "vafp ";
    if (st & EGL_VG_COLORSPACE_LINEAR_BIT) oss << "vcl ";
  }

  oss << inf.bufSize() << "-bit (";

  switch (inf.colorBufType()) {
  case EGL_RGB_BUFFER:
    oss << "R" << inf.redSize() << " G" << inf.greenSize() << " B"
        << inf.blueSize();
    break;
  case EGL_LUMINANCE_BUFFER: oss << "L" << inf.lumSize(); break;
  }

  if (auto a = inf.alphaSize()) oss << " A" << a;

  oss << ")";

  if (auto d = inf.depthSize()) oss << " D" << d;
  if (auto s = inf.stencilSize()) oss << " S" << s;
  if (auto m = inf.alphaMaskSize()) oss << " AM" << m;

  {
    auto n = inf.sampleBufs();
    auto s = inf.samples();

    if (n || s) {
      oss << " ";
      if (n != 1) oss << n;
      oss << "MS" << s;
    }
  }

  if (extra) {
    oss << " max(" << inf.maxPbufWidth() << "x" << inf.maxPbufHeight() << " | "
        << inf.maxPbufPixels() << "px)";

    oss << " swapint" << inf.minSwapInterval() << "-" << inf.maxSwapInterval();

    if (inf.bindToTexRgb()) oss << " bindRGB";
    if (inf.bindToTexRgba()) oss << " bindRGBA";

    switch (inf.transparency()) {
    case EGL_NONE: oss << " opaque"; break;
    case EGL_TRANSPARENT_RGB:
      oss << " transpRGB R" << inf.transpR() << " G" << inf.transpG() << " B"
          << inf.transpB();
      break;
    }

    if (inf.nativeRenderable()) oss << " native";

    // oss << " nvid" << inf.nativeVisualId() << " nvt" << inf.nativeVisualType();

    oss << " level" << inf.level();

    if (auto rt = inf.renderableType(); rt) {
      oss << " (rt";
      if (rt & EGL_OPENGL_BIT) oss << " openGL";
      if (rt & EGL_OPENGL_ES_BIT) oss << " GLES";
      if (rt & EGL_OPENGL_ES2_BIT) oss << " GLES2";
      if (rt & EGL_OPENVG_BIT) oss << " openVG";
      oss << ")";
    }

    if (auto cfm = inf.conformant(); cfm) {
      oss << " (cf";
      if (cfm & EGL_OPENGL_BIT) oss << " openGL";
      if (cfm & EGL_OPENGL_ES_BIT) oss << " GLES";
      if (cfm & EGL_OPENGL_ES2_BIT) oss << " GLES2";
      if (cfm & EGL_OPENVG_BIT) oss << " openVG";
      oss << ")";
    }
  }

  return oss.str();
}
} // namespace std
