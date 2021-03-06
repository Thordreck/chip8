#!/bin/bash

if [ -z "${CHIP8_ROOT_DIR}" ]; then
    echo "CHIP8_ROOT_DIR is empty. Please set this environment variable."
    echo "If you are at the project root directory, you can use the following command:"
    echo "   export CHIP8_ROOT_DIR=\$(pwd)"
    return
fi

function build_project()
{
    cd "${CHIP8_ROOT_DIR}"
    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=True \
          -DBUILD_INTERPRETER=True \
          -DBUILD_TESTS=True \
          -DBUILD_GUI=True \
          -DCMAKE_BUILD_TYPE=Debug \
          -DCLANG_TIDY=True \
          -DIWYU=True \
          ..
    make -j8
    cp compile_commands.json ..
}

if [[ "$1" == "gcc" ]]; then
    export CXX=g++
    export CC=gcc
    build_project

elif [[ "$1" == "clang" ]]; then
    export CXX=clang++
    export CC=clang
    build_project

else
    echo "Error: Provide gcc or clang argument"
    return
fi

