#!/bin/bash

kicad-split --yes -i stm32-network-rs232.mod -o ./kicad-library/mods/
kicad-split --yes -i stm32-network-rs232.lib -o ./kicad-library/libs/
