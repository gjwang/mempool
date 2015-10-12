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
namespace mempool{
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
    
    virtual ~PoolLock(){
        std::cout << "~PoolLock()" << "\n";
    };
    
private:
    pthread_mutex_t _mutex;
};

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
    
    void* operator new (std::size_t size) throw (std::bad_alloc) = delete;
    void* operator new[] (std::size_t size) throw (std::bad_alloc) = delete;
    void operator delete (void *) = delete;
    void operator delete[] (void *) = delete;
private:
    PoolLock* _lock;
};

}
#endif /* defined(__mempool__PoolLock__) */
