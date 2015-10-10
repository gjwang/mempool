//
//  MemPoolManager.cpp
//  mempool
//
//  Created by gjwang on 10/10/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include "MemPoolManager.h"

namespace mempool{
MemPoolManager* MemPoolManager::GetInstance(){
    //define a static local variable, so that it is leaked,
    //and its destructors are not called at exit.
    static MemPoolManager* mPoolManager = new MemPoolManager();
    return mPoolManager;
};

MemPoolManager::MemPoolManager(){
};

//TODO: InitMemPoolByName is not thread safe now
void MemPoolManager::InitMemPoolByName(std::string memPoolName, size_t itemSize, size_t count, bool isFixedSize){
    
    MemPoolMapType::iterator it = mMemPoolMap.find(memPoolName);
    if (it != mMemPoolMap.end()){
        return;
    }else{
        mMemPoolMap[memPoolName] = new MemPool(itemSize, count, isFixedSize);
    }
    
}

MemPool* MemPoolManager::GetMemPoolByName(std::string& memPoolName){
    MemPoolMapType::iterator it = mMemPoolMap.find(memPoolName);
    if (it != mMemPoolMap.end()) {
        return it->second;
    }else{
        return nullptr;
    }
};
    
void MemPoolManager::DeInitMemPoolByName(std::string& memPoolName){
    MemPoolMapType::iterator it = mMemPoolMap.find(memPoolName);
    if (it != mMemPoolMap.end()) {
        delete it->second;
        mMemPoolMap.erase(memPoolName);
    }
}

void MemPoolManager::DeInitMemPoolAll(){
    MemPoolMapType::iterator it;
    for (it = mMemPoolMap.begin(); it != mMemPoolMap.end(); it++) {
        delete it->second;
    }
    
    mMemPoolMap.clear();
}
    
MemPoolManager::~MemPoolManager(){
    DeInitMemPoolAll();
};

}