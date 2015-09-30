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
#include <vector>

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
        //std::cout << "Lock\n";
        (void) pthread_mutex_lock(&_mutex);
    }
    void Unlock(){
        //std::cout << "UnLock\n";
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
//        _lock->Lock();
//    }
    
    explicit LockScoped(std::shared_ptr<PoolLock> sharelock): _shareLock(sharelock) {
        //_lock->Lock();

        _shareLock->Lock();
    }
    
    ~LockScoped(){
        //_lock->Unlock();
        
        _shareLock->Unlock();
    }
    
private:
    //PoolLock* _lock;
    std::shared_ptr<PoolLock> _shareLock;
};


class MemPool{
public:
    //MemPool();
    MemPool(size_t size, size_t count, bool isFixSize=true);
    
    
    
    uint8_t *GetMem();
    void Release(uint8_t* buf);
    ~MemPool();
    
private:
    size_t totalSize;
    bool isFixedSize;
    
    uint8_t * data;
    typedef std::pair<uint8_t*, bool> bufUsingMap;
    std::vector<bufUsingMap> dataVector;
    
    //PoolLock *_poolLock;
    std::shared_ptr<PoolLock> _shareLock;
};

}//end of namespace

#endif /* defined(__cpp_test__MemPool__) */
