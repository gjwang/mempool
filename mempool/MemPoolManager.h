//
//  MemPoolManager.h
//  mempool
//
//  Created by gjwang on 10/10/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#ifndef __mempool__MemPoolManager__
#define __mempool__MemPoolManager__

#include <stdio.h>
#include <vector>
#include <map>

#include "MemPool.h"

namespace mempool {
    
typedef std::map<std::string, MemPool*> MemPoolMapType;
    
class MemPoolManager{
public:
    static MemPoolManager* GetInstance();
    
    MemPoolManager();

    void InitMemPoolByName(std::string memPoolName, size_t itemSize, size_t count, bool isFixedSize=true);
    MemPool *GetMemPoolByName(std::string& memPoolName);

    void DeInitMemPoolByName(std::string& memPoolName);
    void DeInitMemPoolAll();
    
    ~MemPoolManager();
    
private:
    
    MemPoolMapType mMemPoolMap;
};
}

#endif /* defined(__mempool__MemPoolManager__) */
