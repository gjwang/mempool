//
//  threadPosixLockPerfTest.cpp
//  mempool
//
//  Created by gjwang on 10/24/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#include "threadPosixLockPerfTest.h"
#include "PoolLock.h"

#ifndef _INT64_T
#define _INT64_T
typedef	long long		int64_t;
#endif

#define NUM_THREADS (2)

static int64_t  globalCount = 0;
static const int64_t  addPerforTimes = 100000000;
static const int testTimes = 10;

void *threadNonSyncwork(void *argument){
    for (int64_t i = 0; i < addPerforTimes; i++) {
        globalCount++;
    }
    return NULL;
}

mempool::PoolLock lock;
void *threadSyncwork(void *argument){
    for (int64_t i = 0; i < addPerforTimes; i++) {
        mempool::LockScoped ls(&lock);
        globalCount++;
    }
    return NULL;
}

void threadLockPerfTest(void *(*threadWorkerFunc)(void *)){
    pthread_t threads[NUM_THREADS];

    struct timeval timeBegin;
    struct timeval timeEnd;
    
    globalCount = 0;
    int index;
    gettimeofday(&timeBegin, NULL);
    
    for (index = 0; index < NUM_THREADS; ++index) {
        pthread_create(&threads[index], NULL, threadWorkerFunc, NULL);
    }
    
    for (index = 0; index < NUM_THREADS; ++index) {
        pthread_join(threads[index], NULL);
    }
    gettimeofday(&timeEnd, NULL);
    
    std::cout << std::setw(8) <<
    (timeEnd.tv_sec - timeBegin.tv_sec)*1000000 +
    (timeEnd.tv_usec-timeBegin.tv_usec) << " ";
}

int main(int argc, const char * argv[]) {
    int i;
    std::cout << "thread Non sync Performance: usec" << std::endl;
    for (i = 0; i<testTimes; i++) {
        threadLockPerfTest( threadNonSyncwork );
    }
    std::cout << std::endl;
    
    std::cout << "thread Posix sync Performance: usec" << std::endl;
    for (i = 0; i<testTimes; i++) {
        threadLockPerfTest( threadSyncwork );
    }
    std::cout << std::endl;

}
