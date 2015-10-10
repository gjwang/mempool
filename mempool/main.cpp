//
//  main.cpp
//  cpp_test
//
//  Created by gjwang on 9/29/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include <sys/time.h>
#include <time.h>

#include <iostream>
#include "MemPool.h"
#include "MemPoolManager.h"

using namespace mempool;

int main(int argc, const char * argv[]) {
    struct timeval timeBegin;
    struct timeval timeEnd;
    
    size_t itemSize = 1024;
    size_t itemNum = 10;
    
    std::string memPoolName_1("VideoFrameMemPool_itemSize_itemNum_1");
    std::string memPoolName_2("VideoFrameMemPool_itemSize_itemNum_2");

    
    //std::shared_ptr<mempool::MemPool> pMemPool = std::make_shared<mempool::MemPool>(itemSize, itemNum);
    //mempool::MemPool *pMemPool = mempool::MemPool::GetIntance();
    
    MemPoolManager * mpManager = MemPoolManager::GetInstance();
    mpManager->InitMemPoolByName(memPoolName_1, itemSize, itemNum);
    mpManager->InitMemPoolByName(memPoolName_2, itemSize, itemNum);

    mempool::MemPool *pMemPool = mpManager->GetMemPoolByName(memPoolName_1);
    
    
    
    int testTimes = 1000000;
    gettimeofday(&timeBegin, NULL);
    for (int i = 0; i < testTimes; i++) {
        uint8_t * buf = pMemPool->GetMem();
        pMemPool->Release(buf);
    }
    gettimeofday(&timeEnd, NULL);

    std::cout << "memPool used time=" <<
        (timeEnd.tv_sec - timeBegin.tv_sec)*1000000 +
        (timeEnd.tv_usec-timeBegin.tv_usec) << "usec" << std::endl;
    

    gettimeofday(&timeBegin, NULL);
    for (int i = 0; i < testTimes; i++) {
        uint8_t * buf = (uint8_t*)malloc(itemSize);
        free(buf);
    }
    gettimeofday(&timeEnd, NULL);
    
    std::cout << "malloc  used time=" <<
    (timeEnd.tv_sec - timeBegin.tv_sec)*1000000 +
    (timeEnd.tv_usec-timeBegin.tv_usec) << "usec" << std::endl;
    
    
    //mpManager->DeInitMemPoolByName(memPoolName_1);
    //mpManager->DeInitMemPoolByName(memPoolName_2);
    mpManager->DeInitMemPoolAll();
    
    //std::cout << "mpManager->~MemPoolManager()" << std::endl;
    //mpManager->~MemPoolManager();
    
    return 0;
}
