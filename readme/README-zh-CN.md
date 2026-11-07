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

## 文档
SimplePool 支持通过Doxygen生成文档。

运行：
```
doxygen
```

文档现已位于build/docs/html目录中！