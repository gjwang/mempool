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

#include "PoolLock.h"

namespace mempool {

class MemPool{
public:
    //MemPool();
    MemPool(size_t size, size_t count, bool isFixSize=true);
    
    //MemPool* GetIntance();
    
    
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
