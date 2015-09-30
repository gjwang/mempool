//
//  MemPool.cpp
//  cpp_test
//
//  Created by gjwang on 9/29/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include "MemPool.h"
#include <iostream>
//using namespace std;

namespace mempool{

//MemPool::MemPool():
//        /*_poolLock(new PoolLock()),*/
//        _shareLock(std::make_shared<PoolLock>()),
//        data(nullptr){
//    std::cout << "MemPool Constructor\n";
//            
//};

MemPool::MemPool(size_t size, size_t count, bool isFixedSize):
    totalSize(size*count),
    isFixedSize(isFixedSize),
    data(nullptr),
    _shareLock(std::make_shared<PoolLock>()) {
    if (totalSize <= 0) {
        std::cout << "size*count must greater than zero!";
    }
    
    uint8_t * data = (uint8_t*)malloc(totalSize);
    for (size_t i = 0; i < count; i++) {
        dataVector.push_back( std::make_pair(data + i*size, false) );
    }
        
    std::cout << "dataVector size=" << dataVector.size() << "\n";
    
}
    
uint8_t* MemPool::GetMem(){
    LockScoped lc(_shareLock);
    std::vector<bufUsingMap>::iterator it = dataVector.begin();
    for (; it != dataVector.end(); it++) {
        if (!it->second) {
            it->second = true;
            return it->first;
        }
    }

    return nullptr;
};

void MemPool::Release(uint8_t* buf){
    LockScoped lc(_shareLock);
    std::vector<bufUsingMap>::iterator it = dataVector.begin();
    for (; it != dataVector.end(); it++) {
        if (it->first == buf) {
            it->second = false;
        }
    }
};
    
MemPool::~MemPool(){
    std::cout << "MemPool::~MemPool()\n";
    
    dataVector.clear();
    if (data) {
        free(data);
        data = nullptr;
    }
    
    //delete _poolLock;
};

}//end of namepace
