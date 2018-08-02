#if !defined(_JS)

#include "glad.hpp"

#include <cstdarg>
#include <iostream>
#include <string>
#include <unordered_map>

#if defined(_DEBUG_DIAGNOSTIC)
static std::unordered_map<unsigned int, std::string> s_enumNames, s_enumFlags;
static std::unordered_map<std::string, std::vector<glt>> s_funcSigs;

static std::vector<glt> s_bad;

std::string glEnumName(unsigned int val) {
  if (s_enumNames.empty()) {
#include "initGLNames.hpp"
  }

  return s_enumNames.at(val);
}

const std::vector<glt> &glFuncSig(const std::string &name) {
  if (s_funcSigs.empty()) {
#include "initGLSigs.hpp"
  }

  auto it = s_funcSigs.find(name);

  if (it == s_funcSigs.end()) return s_bad;

  return it->second;
}

static void printCall(const char *name, va_list argv) {
  std::string nameStr(name);

  const std::vector<glt> &sig = glFuncSig(name);
  std::cerr << nameStr << "(";

  std::vector<std::string> args;

  for (auto it = sig.begin(), end = sig.end(); it != end; ++it) {
    if (it == sig.begin()) continue;

    switch (*it) {
    case glt::gBitfield: break;
    case glt::gBoolean:
      args.push_back(va_arg(argv, int) ? "true" : "false");
      break;
    case glt::gClampf:
      args.push_back(std::to_string(va_arg(argv, double)));
      break;
    case glt::gDouble:
      args.push_back(std::to_string(va_arg(argv, GLdouble)));
      break;
    case glt::gEBUGPROC: break;
    case glt::gEBUGPROCKHR: break;
    case glt::gEglClientBufferEXT: break;
    case glt::gEglImageOES: break;
    case glt::gEnum: args.push_back(glEnumName(va_arg(argv, GLenum))); break;
    case glt::gFloat:
      args.push_back(std::to_string(va_arg(argv, GLdouble)));
      break;
    case glt::gInt: args.push_back(std::to_string(va_arg(argv, GLint))); break;
    case glt::gInt64EXT:
      args.push_back(std::to_string(va_arg(argv, GLint64EXT)));
      break;
    case glt::gIntptr:
      args.push_back(std::to_string(va_arg(argv, GLintptr)));
      break;
    case glt::gSizei:
      args.push_back(std::to_string(va_arg(argv, GLsizei)));
      break;
    case glt::gSizeiptr:
      args.push_back(std::to_string(va_arg(argv, GLsizeiptr)));
      break;
    case glt::gSync: break;
    case glt::gULKANPROCNV: break;
    case glt::gUint:
      args.push_back(std::to_string(va_arg(argv, GLuint)));
      break;
    case glt::gUint2: break;
    case glt::gUint64:
      args.push_back(std::to_string(va_arg(argv, GLuint64)));
      break;
    case glt::gUint64EXT:
      args.push_back(std::to_string(va_arg(argv, GLuint64EXT)));
      break;
    case glt::gVoid: args.push_back("void"); break;
    case glt::pcChar:
    case glt::pBoolean:
    case glt::pChar:
    case glt::pEnum:
    case glt::pFloat:
    case glt::pInt:
    case glt::pInt64:
    case glt::pInt64EXT:
    case glt::pSizei:
    case glt::pUbyte:
    case glt::pUint:
    case glt::pUint64:
    case glt::pVoid:
    case glt::pcDouble:
    case glt::pcEnum:
    case glt::pcFloat:
    case glt::pcInt:
    case glt::pcInt64EXT:
    case glt::pcSizei:
    case glt::pcUbyte:
    case glt::pcUint:
    case glt::pcUint64:
    case glt::pcUint64EXT:
    case glt::pcVoid:
    case glt::pcpcChar:
    case glt::pcpcVoid:
    case glt::ppVoid:
    case glt::ppcChar:
      args.push_back("(*) " + std::to_string(reinterpret_cast<std::intptr_t>(
                                  va_arg(argv, void *))));
      break;
    default: args.push_back("???");
    }
  }

  bool first = true;

  for (auto arg : args) {
    if (first)
      first = false;
    else
      std::cerr << ", ";
    std::cerr << arg;
  }

  std::cerr << ")";
}

static bool s_diagOn = true;

void glPreCallProc(const char *name, void *, int argc, ...) {
  if (!s_diagOn) return;

  va_list argv;
  va_start(argv, argc);
  printCall(name, argv);
  va_end(argv);
  std::cerr << std::endl;
}

void glPostCallProc(const char *name, void *, int argc, ...) {
  unsigned int err = glad_glGetError();

  if (err != GL_NO_ERROR) {
    std::cerr << "\e[1m";
    va_list argv;
    va_start(argv, argc);
    printCall(name, argv);
    va_end(argv);
    std::cerr << ": \e[38;5;1mERROR:\e[0m " << glEnumName(err) << std::endl;
    abort();
  }
}

void glDiagOff() { s_diagOn = false; }

void glDiagOn() { s_diagOn = true; }
#endif

#endif // _JS
