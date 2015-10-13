//
//  main.cpp
//  cpp_test
//
//  Created by gjwang on 9/29/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

#include <iostream>
#include "MemPool.h"
#include "MemPoolManager.h"

using namespace mempool;

#define NUM_THREADS 4

volatile int64_t globalCount = 0;
const int addPerforTimes = 1000000000;
static PoolLock addLock;
void *perform_work(void *argument)
{
    for (int i = 0; i < addPerforTimes; i++) {
        LockScoped ls(&addLock);
        globalCount++;
    }
    
    return NULL;
}

int main(int argc, const char * argv[]) {
    struct timeval timeBegin;
    struct timeval timeEnd;
    
    size_t itemSize = 1024;
    size_t itemNum = 10;
    
    std::string memPoolName_1("VideoFrameMemPool_itemSize_itemNum_1");
    std::string memPoolName_2("VideoFrameMemPool_itemSize_itemNum_2");

    
    //std::shared_ptr<mempool::MemPool> pMemPool = std::make_shared<mempool::MemPool>(itemSize, itemNum);
    //mempool::MemPool *pMemPool = mempool::MemPool::GetIntance();
    
    MemPoolManager * mpManager = MemPoolManager::getInstance();
    mpManager->InitMemPoolByName(memPoolName_1, itemSize, itemNum);
    mpManager->InitMemPoolByName(memPoolName_2, itemSize, itemNum);

    mempool::MemPool *pMemPool = mpManager->getMemPoolByName(memPoolName_1);
    
    int a = 0;
    __sync_fetch_and_add(&a,10);
    std::cout << a << std::endl;
    
    const int testTimes = 1000000;
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
    

    //-------threads test CASLock and mutexLock---------------------
    
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int result_code, index;
    
    gettimeofday(&timeBegin, NULL);
    for (index = 0; index < NUM_THREADS; ++index) {
        thread_args[index] = index;
        result_code = pthread_create(&threads[index], NULL, perform_work, (void *) &thread_args[index]);
        assert(0 == result_code);
    }
    
    // wait for each thread to complete
    for (index = 0; index < NUM_THREADS; ++index) {
        // block until thread 'index' completes
        result_code = pthread_join(threads[index], NULL);
        //printf("In main: thread %d has completed\n", index);
        assert(0 == result_code);
    }
    
    gettimeofday(&timeEnd, NULL);
    std::cout << "threads add used time=" <<
    (timeEnd.tv_sec - timeBegin.tv_sec)*1000000 +
    (timeEnd.tv_usec-timeBegin.tv_usec) << "usec" << std::endl;
    
    std::cout << "globalCount=" << globalCount << std::endl;;
    printf("In main: All threads completed successfully\n");
    return 0;
}
