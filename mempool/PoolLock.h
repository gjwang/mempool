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
        //std::cout << "PoolLock() this=" << this << "\n";
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

    }
        
    void Lock(){
        while (!__sync_bool_compare_and_swap(&isLocking, false, true)) {
            //TODO: add atomic_refcount, to cancel acquring the lock, to prevent deadlock
            usleep(100);
        };
        //TODO: need to add memory barrier?
    }
    
    void Unlock(){
        while (!__sync_bool_compare_and_swap(&isLocking, true, false)) {
            //std::cout << "should not happen!";
            //TODO: add atomic_refcount, to cancel acquring the lock, to prevent deadlock
            usleep(1);
        };
        //TODO: need to add memory barrier?
    }
    
    ~CASLock(){
    };
        
private:
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
