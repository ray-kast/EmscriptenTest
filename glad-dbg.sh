#!/usr/bin/env sh

glad --out-path . --api gles2=2.0 --profile core --generator c-debug --spec gl
./gen-debug-info.sh