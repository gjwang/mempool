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

using namespace mempool;

int main(int argc, const char * argv[]) {
    struct timeval timeBegin;
    struct timeval timeEnd;
    
//    ts.tv_sec = tv.tv_sec + (milliseconds / 1000);
//    ts.tv_nsec = tv.tv_usec * 1000 + (milliseconds % 1000) * 1000000;
    
    size_t itemSize = 10240;
    size_t itemNum = 10;
    
    std::shared_ptr<mempool::MemPool> pMemPool = std::make_shared<mempool::MemPool>(itemSize, itemNum);
    
    int testTimes = 10000000;
    gettimeofday(&timeBegin, NULL);
    for (int i = 0; i < testTimes; i++) {
        uint8_t * buf = pMemPool->GetMem();
        //std::cout << "getMem=" << (void*)buf << '\n';
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
    
    return 0;
}
