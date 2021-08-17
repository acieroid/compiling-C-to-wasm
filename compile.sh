#!/bin/sh -x
SRC=$@
SYSROOT=$(pwd)/wasi
ARGS="-O1 -lm"
# orbs-wasm data uses
# ARGS="-O1 -fno-inline-functions -lm"

WASMARGS="$ARGS -Wl,--demangle -Wl,--export-all"
# ARGS=-nostartfile -WL,--no-entry -WL,--export-all -WL,--import-memory

# Compile into webassembly
clang --target=wasm32-unknown-wasi --sysroot $SYSROOT $WASMARGS $SRC -o $SRC.wasm
wasm2wat $SRC.wasm --output=$SRC.wat
