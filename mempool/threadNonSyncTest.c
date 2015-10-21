//
//  threadNonSyncTest.c
//  mempool
//
//  Created by gjwang on 10/21/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>

#include "threadNonSyncTest.h"

#ifndef _INT64_T
#define _INT64_T
typedef	long long		int64_t;
#endif

#define NUM_THREADS (2)

static int64_t  globalCount = 0;
static const int64_t  addPerforTimes = 1000000;
static const int testTimes = 1000;

void *thread_work(void *argument)
{
    for (int64_t i = 0; i < addPerforTimes; i++) {
        globalCount++;
    }
    return NULL;
}

void threadNonSyncTest(){
    pthread_t threads[NUM_THREADS];

    globalCount = 0;
    int index;
    
    for (index = 0; index < NUM_THREADS; ++index) {
        pthread_create(&threads[index], NULL, thread_work, NULL);
    }
    
    for (index = 0; index < NUM_THREADS; ++index) {
        pthread_join(threads[index], NULL);
    }
    
    printf("globalCount=%lld\n", globalCount);
}

//int main(int argc, const char * argv[]) {
//    int i;
//    for (i = 0; i<testTimes; i++) {
//        threadNonSyncTest();
//        printf("%lld\n", globalCount);
//
//    }
//}
