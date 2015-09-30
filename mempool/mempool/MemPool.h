//
//  MemPool.h
//  cpp_test
//
//  Created by gjwang on 9/29/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#ifndef __cpp_test__MemPool__
#define __cpp_test__MemPool__

#include "stdio.h"
#include <iostream>

namespace mempool {
class PoolLock{
public:
    explicit PoolLock(){
        std::cout << "PoolLock() this=" << this << "\n";
        pthread_mutexattr_t attr;
        (void) pthread_mutexattr_init(&attr);
        (void) pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        (void) pthread_mutex_init(&_mutex, &attr);
    }
    
    void Lock(){
        std::cout << "Lock\n";
        (void) pthread_mutex_lock(&_mutex);
    }
    void Unlock(){
        std::cout << "UnLock\n";
        (void) pthread_mutex_unlock(&_mutex);
    }
    
    virtual ~PoolLock(){
        std::cout << "~PoolLock()" << "\n";
    };

private:
    pthread_mutex_t _mutex;
};

class  LockScoped {
public:
//    explicit LockScoped(PoolLock* lock): _lock(lock) {
//        std::cout << "_lock->Lock()=" << _lock << "\n";
//        _lock->Lock();
//    }
    
    explicit LockScoped(std::shared_ptr<PoolLock> sharelock): _shareLock(sharelock) {
        //_lock->Lock();

        //std::cout << "_shareLock=" << _shareLock.get() << "\n";
        _shareLock->Lock();
    }
    
    ~LockScoped(){
        //_lock->Unlock();
        
        //sstd::cout << "_shareLock->Unlock()\n";
        _shareLock->Unlock();
    }
    
private:
    //PoolLock* _lock;
    std::shared_ptr<PoolLock> _shareLock;
};


class MemPool{
public:
    MemPool();
    void * GetMem();
    ~MemPool();

private:
    //PoolLock *_poolLock;
    std::shared_ptr<PoolLock> _shareLock;
};

}//end of namespace

#endif /* defined(__cpp_test__MemPool__) */
