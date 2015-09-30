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

MemPool::MemPool():
        /*_poolLock(new PoolLock()),*/
        _shareLock(std::make_shared<PoolLock>()){
    std::cout << "MemPool Constructor\n";
};

void *MemPool::GetMem(){
//    {
//        LockScoped ls(_poolLock);
//    }
    
    {
        LockScoped lc(_shareLock);
    }
    

    return nullptr;
};
    
MemPool::~MemPool(){
    std::cout << "MemPool::~MemPool()\n";
    //delete _poolLock;
};

}//end of namepace
