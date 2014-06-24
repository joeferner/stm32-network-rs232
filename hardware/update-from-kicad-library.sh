#!/bin/bash

kicad-update -i stm32-network-rs232.mod.list -o stm32-network-rs232.mod --basedir ./kicad-library
kicad-update -i stm32-network-rs232.lib.list -o stm32-network-rs232.lib --basedir ./kicad-library
