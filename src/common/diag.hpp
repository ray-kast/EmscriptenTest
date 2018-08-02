#pragma once

#include <string>

void info(const std::string &);

void warn(const std::string &);

void err(const std::string &);

[[noreturn]] void die(const std::string &);

#if defined(_DEBUG_DIAGNOSTIC)
#define DIAG_IFEL(t, f) t
#else
#define DIAG_IFEL(t, f) f
#endif

#define DIAG_IF(t) DIAG_IFEL(t, )
