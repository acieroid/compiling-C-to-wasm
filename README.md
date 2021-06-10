To compile:
  ./compile.sh hello.c

The wasi directory contains the WebAssembly System Interface, downloaded from: https://github.com/WebAssembly/wasi-sdk/releases/tag/wasi-sdk-12

In case clang complains of a missing .a file, the corresponding file is libclang_rt.builtins-wasm32.a and can be copied in the relevant Clang directory (for example: /usr/lib/clang/12.0.0/lib/wasi/libclang_rt.builtins-wasm32.a)
