#!/usr/bin/env zsh

setopt extendedglob
clang-format -i src/**/{*.cpp,(*~(initGLNames|initGLSigs)).hpp,*.vert,*.frag}
