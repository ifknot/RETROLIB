#!/bin/bash
echo "Running CMAKE for OW2..."

cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS -D CMAKE_SYSTEM_PROCESSOR=I86 -S. -B ../bin
