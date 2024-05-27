#pragma once

#include <iostream>
#include <cassert>
#include <vector>


#define AlignNum  8
#define _RoundUp(size, alignNum)     \
    (((size) + (alignNum) - 1) & ~((alignNum) - 1))


class Fixed_Mem_Pool{
public:
    Fixed_Mem_Pool(size_t psize, size_t objsize);
    ~Fixed_Mem_Pool();

    struct Chunk{                  
        Chunk *next;
    };

    void* palloc();
    void pfree(void *obj);
    
private:
    void *_memory = nullptr;       // 申请的过量内存块
    Chunk *_freelist = nullptr;    // 空闲内存块链表
    size_t _remainBytes = 0;       // 内存池内存剩余的字节
    size_t _objSize = 0;           // 定长类型的大小
    size_t _pool_size;             // 内存池的内存大小
    std::vector<void*> _total;     // 保存申请的内存块(用于最后释放)
};


