#!/bin/bash -e

PROJECT_NAME=$(sed -n -e 's/PROJECT(\(.*\))/\1/p' CMakeLists.txt)
echo "Building project ${PROJECT_NAME}"

function get_flash_and_ram {
  local line=$(grep 'SET(STM32_FLASH_SIZE' CMakeLists.txt)
  regex="SET\(STM32_FLASH_SIZE \"([0-9]*)\"\)"
  if [[ $line =~ $regex ]]; then
    FLASH="${BASH_REMATCH[1]}"
  else
    FLASH=0
  fi

  local line=$(grep 'SET(STM32_RAM_SIZE' CMakeLists.txt)
  regex="SET\(STM32_RAM_SIZE \"([0-9]*)\"\)"
  if [[ $line =~ $regex ]]; then
    RAM="${BASH_REMATCH[1]}"
  else
    RAM=0
  fi
}

function create_version_h {
  GIT_HASH=`git rev-parse HEAD`

  cat > ./version.h <<EOL
#ifndef _VERSION_H_
#define _VERSION_H_

#define GIT_HASH "${GIT_HASH}"

#endif
EOL
}

function build {
  mkdir -p build
  cd build
  if [[ -f ${PROJECT_NAME}.elf ]]; then
    rm ${PROJECT_NAME}.elf
  fi
  make ${PROJECT_NAME}.bin && \
  make ${PROJECT_NAME}.list
}

function print_size {
  arm-none-eabi-size ${PROJECT_NAME}.elf | tee size.report

  TEXT_SIZE=`cat size.report | tail -1 | cut -f 1 | tr -d ' '`
  DATA_SIZE=`cat size.report | tail -1 | cut -f 2 | tr -d ' '`
  BSS_SIZE=`cat size.report | tail -1 | cut -f 3 | tr -d ' '`

  RAM_ALL_KB=$(echo "scale=1; ${RAM}/1024" | bc -l)
  FLASH_ALL_KB=$(echo "scale=1; ${FLASH}/1024" | bc -l)

  TEXT_SIZE_KB=$(echo "scale=1; ${TEXT_SIZE}/1024" | bc -l)
  DATA_SIZE_KB=$(echo "scale=1; ${DATA_SIZE}/1024" | bc -l)
  BSS_SIZE_KB=$(echo "scale=1; ${BSS_SIZE}/1024" | bc -l)

  FLASH_TOTAL=$(echo "${TEXT_SIZE}+${DATA_SIZE}" | bc)
  RAM_TOTAL=$(echo "${DATA_SIZE}+${BSS_SIZE}" | bc)
  FLASH_REMAINING=$(echo "${FLASH}-${FLASH_TOTAL}" | bc)
  RAM_REMAINING=$(echo "${RAM}-${RAM_TOTAL}" | bc)

  FLASH_TOTAL_KB=$(echo "scale=1; ${FLASH_TOTAL}/1024" | bc -l)
  RAM_TOTAL_KB=$(echo "scale=1; ${RAM_TOTAL}/1024" | bc -l)
  FLASH_REMAINING_KB=$(echo "scale=1; ${FLASH_REMAINING}/1024" | bc -l)
  RAM_REMAINING_KB=$(echo "scale=1; ${RAM_REMAINING}/1024" | bc -l)

  echo ""
  echo "ram = ${RAM_ALL_KB}kB"
  echo "flash = ${FLASH_ALL_KB}kB"
  echo ""
  echo "flash = text (${TEXT_SIZE_KB}kB) + data (${DATA_SIZE_KB}kB) = ${FLASH_TOTAL_KB}kB (remaining: ${FLASH_REMAINING_KB}kB)"
  echo "  ram = data (${DATA_SIZE_KB}kB) +  bss (${BSS_SIZE_KB}kB) = ${RAM_TOTAL_KB}kB (remaining: ${RAM_REMAINING_KB}kB)"
  echo ""
}

get_flash_and_ram
create_version_h
build
print_size
