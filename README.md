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


# Example:
Here are some examples 

### C
```c
#include <stdio.h>
#include <string.h>

#include "simple_pool.h"

typedef struct {int i; char c[10];} Example;

int main(){

    SimplePool* pool = simple_pool_init(sizeof(Example), 8);

    Example* r = (Example*)simple_pool_alloc(pool);
    r->i = 42;
    strcpy(r->c, "hello");
    printf("%d %s\n", r->i, r->c);
    
    return 0;
}
```
Then you should see the output:
```bash
42 hello
```

### Python
```python
import ctypes
import os

HERE = os.path.dirname(os.path.abspath(__file__))
LIB_PATH = os.path.join(HERE, "libSimplePool.dylib")
lib = ctypes.CDLL(LIB_PATH)

lib.simple_pool_init.argtypes    = [ctypes.c_size_t, ctypes.c_size_t]
lib.simple_pool_init.restype     = ctypes.c_void_p

lib.simple_pool_alloc.argtypes   = [ctypes.c_void_p]
lib.simple_pool_alloc.restype    = ctypes.c_void_p

lib.simple_pool_dealloc.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.simple_pool_dealloc.restype  = None

lib.simple_pool_destroy.argtypes = [ctypes.c_void_p]
lib.simple_pool_destroy.restype  = None

pool = lib.simple_pool_init(640, 10)

p = lib.simple_pool_alloc(pool)

data = b"hello memory pool"
ctypes.memmove(p, data, len(data))

buf = ctypes.create_string_buffer(len(data))
ctypes.memmove(buf, p, len(data))
print("OK:", buf.raw)

lib.simple_pool_dealloc(pool, p)
lib.simple_pool_destroy(pool)
```
Then you should see the output:
```bash
OK: b'hello memory pool'
```

## Document
SimplePool supports generating documentation through Doxygen.

Run:
```bash
doxygen
```

The Document is now located in the build/docs/html directory!