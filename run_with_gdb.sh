#!/usr/bin/bash

gdb -batch -ex "run" -ex "where" --args ${@:1}
