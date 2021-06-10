#!/bin/sh -x
SRC=$@
SYSROOT=$(pwd)/wasi
ARGS="-O1"
WASMARGS="$ARGS -Wl,--demangle -Wl,--export-all"
# ARGS=-nostartfile -WL,--no-entry -WL,--export-all -WL,--import-memory

# Compile into webassembly
clang --target=wasm32-unknown-wasi --sysroot $SYSROOT $WASMARGS $SRC -o $SRC.wasm
