#!/usr/bin/env zsh

cd "$(dirname "$0")"
setopt extendedglob
clang-format -i src/**/{*.cpp,(*~(initGLNames|initGLSigs)).hpp,*.vert,*.frag}
