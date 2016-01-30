#! usr/bin/bash
# -DHANA_INCLUDE_DIR = path to boost hana include dir
# -DHANA_INCLUDE_DIR = path to llvm include dir
# -DHANA_INCLUDE_DIR = path to llvm-c include dir
# Exmple:
# cmake -DHANA_INCLUDE_DIR=/home/alnen/Projects/hana/include -DLLVM_INCLUDE_DIR=/usr/include/llvm-3.7 -DLLVM_C_INCLUDE_DIR=/usr/include/llvm-c-3.7 .
cmake -DHANA_INCLUDE_DIR= -DLLVM_INCLUDE_DIR= -DLLVM_C_INCLUDE_DIR= .
