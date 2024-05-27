#include "Fixed_Mem_Pool.h"


Fixed_Mem_Pool::Fixed_Mem_Pool(size_t psize, size_t objsize)
    :_pool_size(psize), _remainBytes(psize){
	_objSize = _RoundUp(objsize, AlignNum);
    _memory = (void*)malloc(_pool_size);
    if (!_memory){
        throw std::bad_alloc();
    }
    _total.push_back(_memory);
}


Fixed_Mem_Pool::~Fixed_Mem_Pool(){
    for(auto it : _total){
        free(it);
    }
}


void* Fixed_Mem_Pool::palloc(){
    void *obj = nullptr;

    if(_freelist){
        obj = _freelist;
        _freelist = objNext(_freelist);
    }else{
        if(_remainBytes < _objSize){
            _memory = (void*)malloc(_pool_size);
            if (!_memory){
                throw std::bad_alloc();
            }
            _total.push_back(_memory);
            _remainBytes = _pool_size;
        }
        
        obj = _memory;
        _memory += _objSize;
        _remainBytes -= _objSize;
    }
    return obj;
}


void Fixed_Mem_Pool::pfree(void *obj){
    objNext(obj) = _freelist;
    _freelist = obj;
}


void*& Fixed_Mem_Pool::objNext(void* obj){
    return *(void**)obj;
}



