# SimplePool

Languages
：[简体中文](readme/README-zh-CN.md)
| English

A simple memory pool written in C++ that can be called by other languages.

## Build
Run:
```bash
mkdir build && cd build
cmake ..
make
```

The header file (simple_pool.h) has been placed in /build/include.

The default library type is a dynamic library. To export a static library, you need to specify `-DEXPORT_SHARED=OFF` for cmake.

## Document
SimplePool supports generating documentation through Doxygen.

Run:
```bash
doxygen
```

The Document is now located in the build/docs/html directory!