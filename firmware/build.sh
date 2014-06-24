#!/bin/bash -e

mkdir -p build
cd build
rm stm32-network-rs232.elf || echo "Cannot remove. stm32-network-rs232.elf not build?"
make stm32-network-rs232.bin && \
make stm32-network-rs232.list
