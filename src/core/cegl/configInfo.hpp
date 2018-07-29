#pragma once

#include "display.hpp"

namespace cegl {
class ConfigInfo {
  const Display *m_disp;
  EGLConfig      m_config;

  EGLint attr(EGLint) const;

public:
  ConfigInfo(const Display &disp, EGLConfig config) :
      m_disp(&disp),
      m_config(config) {}

#define CI_ATTR(name, id)                                                      \
  auto name() const { return attr(id); }

  CI_ATTR(alphaMaskSize, EGL_ALPHA_MASK_SIZE)
  CI_ATTR(alphaSize, EGL_ALPHA_SIZE)
  CI_ATTR(bindToTexRgb, EGL_BIND_TO_TEXTURE_RGB)
  CI_ATTR(bindToTexRgba, EGL_BIND_TO_TEXTURE_RGBA)
  CI_ATTR(blueSize, EGL_BLUE_SIZE)
  CI_ATTR(bufSize, EGL_BUFFER_SIZE)
  CI_ATTR(colorBufType, EGL_COLOR_BUFFER_TYPE)
  CI_ATTR(caveat, EGL_CONFIG_CAVEAT)
  CI_ATTR(id, EGL_CONFIG_ID)
  CI_ATTR(conformant, EGL_CONFORMANT)
  CI_ATTR(depthSize, EGL_DEPTH_SIZE)
  CI_ATTR(greenSize, EGL_GREEN_SIZE)
  CI_ATTR(level, EGL_LEVEL)
  CI_ATTR(lumSize, EGL_LUMINANCE_SIZE)
  CI_ATTR(maxPbufWidth, EGL_MAX_PBUFFER_WIDTH)
  CI_ATTR(maxPbufHeight, EGL_MAX_PBUFFER_HEIGHT)
  CI_ATTR(maxPbufPixels, EGL_MAX_PBUFFER_PIXELS)
  CI_ATTR(maxSwapInterval, EGL_MAX_SWAP_INTERVAL)
  CI_ATTR(minSwapInterval, EGL_MIN_SWAP_INTERVAL)
  CI_ATTR(nativeRenderable, EGL_NATIVE_RENDERABLE)
  CI_ATTR(nativeVisualId, EGL_NATIVE_VISUAL_ID)
  CI_ATTR(nativeVisualType, EGL_NATIVE_VISUAL_TYPE)
  CI_ATTR(redSize, EGL_RED_SIZE)
  CI_ATTR(renderableType, EGL_RENDERABLE_TYPE)
  CI_ATTR(sampleBufs, EGL_SAMPLE_BUFFERS)
  CI_ATTR(samples, EGL_SAMPLES)
  CI_ATTR(stencilSize, EGL_STENCIL_SIZE)
  CI_ATTR(surfType, EGL_SURFACE_TYPE)
  CI_ATTR(transparency, EGL_TRANSPARENT_TYPE)
  CI_ATTR(transpR, EGL_TRANSPARENT_RED_VALUE)
  CI_ATTR(transpG, EGL_TRANSPARENT_GREEN_VALUE)
  CI_ATTR(transpB, EGL_TRANSPARENT_BLUE_VALUE)
};
} // namespace cegl

namespace std {
string to_string(const cegl::ConfigInfo &, bool extra = false);
}
