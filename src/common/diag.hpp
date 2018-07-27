#pragma once

#include <string>

void info(const std::string &);

void warn(const std::string &);

void err(const std::string &);

[[noreturn]] void die(const std::string &);
