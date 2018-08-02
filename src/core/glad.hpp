#pragma once

#if defined(_JS)

#include <GLES2/gl2.h>

#define glEnumName std::to_string

#else

#include <string>
#include <vector>

#include <glad/glad.h>

#if defined(GLAD_DEBUG) && defined(_DEBUG_DIAGNOSTIC)
enum class glt {
  gBitfield,
  gBoolean,
  gClampf,
  gDouble,
  gEBUGPROC,
  gEBUGPROCKHR,
  gEglClientBufferEXT,
  gEglImageOES,
  gEnum,
  gFloat,
  gInt,
  gInt64EXT,
  gIntptr,
  gSizei,
  gSizeiptr,
  gSync,
  gULKANPROCNV,
  gUint,
  gUint2,
  gUint64,
  gUint64EXT,
  gVoid,
  pBoolean,
  pChar,
  pEnum,
  pFloat,
  pInt,
  pInt64,
  pInt64EXT,
  pSizei,
  pUbyte,
  pUint,
  pUint64,
  pVoid,
  pcChar,
  pcDouble,
  pcEnum,
  pcFloat,
  pcInt,
  pcInt64EXT,
  pcSizei,
  pcUbyte,
  pcUint,
  pcUint64,
  pcUint64EXT,
  pcVoid,
  pcpcChar,
  pcpcVoid,
  ppVoid,
  ppcChar,
};

std::string glEnumName(unsigned int);

const std::vector<glt> &glFuncSig(const std::string &);

void glPreCallProc(const char *name, void *func, int argc, ...);

void glPostCallProc(const char *name, void *func, int argc, ...);

static struct _glad_init {
  _glad_init() {
    glad_set_pre_callback(glPreCallProc);
    glad_set_post_callback(glPostCallProc);
  }
} __glad_init__;

void glDiagOff();
void glDiagOn();
#else

#define glEnumName std::to_string

#define glDiagOff()
#define glDiagOn()
#endif

#endif // _JS
