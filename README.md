# 定长内存池
* 采用类模板template，可传入各种自定义类型的内存申请，暂不支持多线程并发

## A、高并发下传统方式的弊端
* 1、频繁的系统调用导致内核态与用户态的切换，造成性能损耗
* 2、频繁使用时增加了系统内存的碎片，降低内存使用效率
* 3、没有垃圾回收机制，容易造成内存泄漏，导致内存枯竭
* 4、内存分配与释放的逻辑在程序中相隔较远时，降低程序的稳定性

## B、解决之道
* 1、使用高性能内存管理组件
* 2、手搓内存池(定长内存池)

## C、内存池如何解决弊端
* 1、高并发时系统调用频繁，降低了系统的执行效率
* √ 内存池提前预先分配大块内存，统一释放，极大的减少了malloc和 free 等函数的调用。
* 2、频繁使用时增加了系统内存的碎片，降低内存使用效率
* √内存池每次请求分配大小适度的内存块，避免了碎片的产生
* 3、没有垃圾回收机制，容易造成内存泄漏
* √在生命周期结束后统一释放内存，完全避免了内存泄露的产生
* 4、内存分配与释放的逻辑在程序中相隔较远时，降低程序的稳定性
* √在生命周期结束后统一释放内存，避免重复释放指针或释放空指针等情况


## 定长内存池解析
1、定长内存池采用类模板，实现不同类型的内存池
```bash
size_t pool_size = 256 * 1024;
Fixed_Mem_Pool<int> TNPool(pool_size);            // int类型
Fixed_Mem_Pool<char> TNPool(pool_size);           // char类型
Fixed_Mem_Pool<float> TNPool(pool_size);          // float类型
Fixed_Mem_Pool<TreeNode> TNPool(pool_size);       // TreeNode类型(自定义结构体)
```

2、定长内存池暴露两个public接口：palloc()、pfree()
```bash
{
    size_t pool_size = 256 * 1024;
    Fixed_Mem_Pool<int> TNPool(pool_size);        // int类型
    int *p = TNPool.palloc();                     // 申请内存
    TNPool.pfree(p);                              // 释放内存(由内部空闲链表管理)
}

离开作用域，内存池自动调用析构释放所有内存块
```

## 环境要求
* Linux
* C++11

## 目录树
```
.
├── src                              源代码
│   ├── Fixed_Mem_Pool_Test.cpp
│   └── Fixed_Mem_Pool.h
├── bin                              可执行文件
│   └── Fixed_Mem_Pool
├── Makefile
├── LICENSE
└── README.md
```

## 结果对比
```bash
Rounds = 128
N = 1000000;

申请次数 = Rounds * N

George@R9000P:~/Projects/Mem_Pool$ ./bin/Fixed_Mem_Pool 
malloc cost time:1.91401s
mem_pool cost time:0.618392s
```

[@George-Hotz](https://github.com/George-Hotz/Fixed_Mem_Pool)
