#!/bin/bash
set -e

if [ "$EUID" -eq 0 ]; then 
  echo "Please do not run as root."
  exit 1
fi

if [ "$#" -ne 1 ]; then
  echo "Usage: ./build.sh <path-to-your-main-config.hal> (e.g. ~/linuxcnc/configs/CNC/CNC.hal)"
  exit 1
fi

mkdir -p cmake-build
cmake -S . -B cmake-build -DHAL2CPP_MAIN_CONFIG_HAL_FILE="$1"
cmake --build cmake-build
sudo cmake --build cmake-build --target install