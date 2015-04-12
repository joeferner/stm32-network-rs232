#!/bin/bash

PROJECT_NAME=$(sed -n -e 's/PROJECT(\(.*\))/\1/p' CMakeLists.txt)
echo "Running GDB ${PROJECT_NAME}"

arm-none-eabi-gdb -tui -ex "target extended-remote localhost:4242" build/${PROJECT_NAME}.elf
