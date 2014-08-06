#!/bin/bash

arm-none-eabi-gdb -ex "target extended-remote localhost:4242" build/stm32-network-rs232.elf
