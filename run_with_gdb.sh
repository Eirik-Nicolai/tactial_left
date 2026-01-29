#!/usr/bin/env sh

gdb -batch -ex "run" -ex "where" --args ${@:1}
