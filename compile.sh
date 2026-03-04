#!/bin/sh
fileName=$1
clang -Xpreprocessor -fopenmp \
  -I/opt/homebrew/opt/libomp/include \
  -L/opt/homebrew/opt/libomp/lib \
  -lomp \
  ${fileName}.c \
  -o ${fileName}