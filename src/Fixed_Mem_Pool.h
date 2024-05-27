#pragma once

#include <iostream>
#include <cassert>
#include <vector>


#define AlignNum  8
#define _RoundUp(size, alignNum)     \
    (((size) + (alignNum) - 1) & ~((alignNum) - 1))


template<class T>
class Fixed_Mem_Pool{
public:
    Fixed_Mem_Pool(size_t size);
    ~Fixed_Mem_Pool();

    struct Chunk{                  
        Chunk *next;
    };

    T* palloc();
    void pfree(T *obj);

private:
    void *_memory = nullptr;       // 申请的过量内存块
    Chunk *_freelist = nullptr;    // 空闲内存块链表
    size_t _remainBytes = 0;       // 内存池内存剩余的字节
    size_t _objSize = 0;           // 定长类型的大小
    size_t _pool_size;             // 内存池的内存大小
    std::vector<void*> _total;     // 保存申请的内存块(用于最后释放)
};

template<class T>
Fixed_Mem_Pool<T>::Fixed_Mem_Pool(size_t size)
    :_pool_size(size), _remainBytes(size){
    _objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
    _objSize = _RoundUp(_objSize, AlignNum);
    _memory = (void*)malloc(_pool_size);
    if (!_memory){
        throw std::bad_alloc();
    }
    _total.push_back(_memory);
}

template<class T>
Fixed_Mem_Pool<T>::~Fixed_Mem_Pool(){
    for(auto it : _total){
        free(it);
    }
}

template<class T>
T* Fixed_Mem_Pool<T>::palloc(){
    T *obj = nullptr;

    if(_freelist){
        obj = (T*)_freelist;
        _freelist = _freelist->next;
    }else{
        if(_remainBytes < _objSize){
            _memory = (void*)malloc(_pool_size);
            if (!_memory){
                throw std::bad_alloc();
            }
            _total.push_back(_memory);
            _remainBytes = _pool_size;
        }
        
        obj = (T*)_memory;
        _memory += _objSize;
        _remainBytes -= _objSize;
    }
    new(obj) T;   //定位new
    return obj;
}


template<class T>
void Fixed_Mem_Pool<T>::pfree(T *obj){
    obj->~T();    //显示调用析构

    Chunk *node = (Chunk*)obj;
    node->next = _freelist;
    _freelist = node;
}


