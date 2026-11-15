# SimplePool

Languages
：[English](../README.md) 
| 简体中文

一个可被其他语言调用的简单内存池。

## 构建
运行：
```bash
mkdir build && cd build 
cmake ..
make
```
头文件（simple_pool.h）已放置在/build/include目录中。
默认的库类型为动态库，为了导出静态库，需要给cmake指定`-DEXPORT_SHARED=OFF`。

## 安装
为了安装SimplePool，请确保已经构建了SimplePool。从你的构建目录（或填写它的路径），运行：
```bash
cmake --install <path-to-build-directory>
```
然后你应该可以把它用于其他目标：
```cmake
find_package(SimplePool)
target_link_libraries(TargetName PRIVATE SimplePool::SimplePool)
```
## 示例
这里是一些示例

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
然后你应该看到输出：
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
然后你应该看到输出：
```bash
OK: b'hello memory pool'
```

## 文档
SimplePool 支持通过Doxygen生成文档。

运行：
```
doxygen
```

文档现已位于build/docs/html目录中！