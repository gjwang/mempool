//
//  PoolLock.h
//  mempool
//
//  Created by gjwang on 10/12/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#ifndef __mempool__PoolLock__
#define __mempool__PoolLock__

#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>

namespace mempool{
#if 0
class PoolLock{
public:
    explicit PoolLock(){
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutex, &attr);
    }
    
    void Lock(){
        pthread_mutex_lock(&_mutex);
    }
    void Unlock(){
        pthread_mutex_unlock(&_mutex);
    }
    
    ~PoolLock(){
        pthread_mutex_destroy(&_mutex);
    };
    
private:
    pthread_mutex_t _mutex;
};

#else
class CASLock{
public:
    explicit CASLock(): isLocking(false){
        pthread_key_create(&_key, NULL);
    }
        
    void Lock(){
        //use lockTimes to let the same thread locks and unlocks multi times
        //like lock{lock{critical_resource}}
        void* pLockTimes = pthread_getspecific(_key);
        size_t lockTimes = 0;
        if(pLockTimes == NULL){
            pthread_setspecific(_key, (void*)lockTimes);
        }else{
            lockTimes = (size_t)pthread_getspecific(_key);
        }
        
        //if lockTimes!=0,
        //it means the current thread is the same thread which already holds the lock
        if (lockTimes == 0) {
            while (!__sync_bool_compare_and_swap(&isLocking, false, true)) {
                usleep(100);
            };
        }
        
        lockTimes++;
        pthread_setspecific(_key, (void*)lockTimes);
        
    }
    
    void Unlock(){
        size_t lockTimes = (size_t)(pthread_getspecific(_key));
        
        lockTimes--;
        pthread_setspecific(_key, (void*)lockTimes);

        if (lockTimes==0) {
            _thread = nullptr;
            while (!__sync_bool_compare_and_swap(&isLocking, true, false)) {
                std::cout << "should not happen!";
                usleep(1);
            };
        }
    }
    
    ~CASLock(){
        pthread_key_delete(_key);
    };
        
private:
    pthread_t _thread;
    pthread_key_t _key;
    bool isLocking;
};
    
typedef CASLock PoolLock;
#endif
    
class  LockScoped {
public:
    explicit LockScoped(PoolLock* lock): _lock(lock) {
        _lock->Lock();
    }
    
    explicit LockScoped(std::shared_ptr<PoolLock> sharelock): _lock(sharelock.get()) {
        _lock->Lock();
    }
    
    ~LockScoped(){
        _lock->Unlock();
    }
    
    //To prevent alloc LockScoped in heap memory,
    //Only alloc LockScoped in stack memory
    void* operator new (std::size_t size) throw (std::bad_alloc) = delete;
    void* operator new[] (std::size_t size) throw (std::bad_alloc) = delete;
    void operator delete (void *) = delete;
    void operator delete[] (void *) = delete;
private:
    PoolLock* _lock;
};

}
#endif /* defined(__mempool__PoolLock__) */
