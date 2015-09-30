//
//  main.cpp
//  cpp_test
//
//  Created by gjwang on 9/29/15.
//  Copyright (c) 2015 gjwang. All rights reserved.
//

#include <iostream>
#include "MemPool.h"

using namespace mempool;

int main(int argc, const char * argv[]) {
    //MemPool p;
    
    std::shared_ptr<mempool::MemPool> pMemPool = std::make_shared<mempool::MemPool>();
    
    std::cout << "GetMem\n";
    pMemPool->GetMem();
    
    pMemPool->GetMem();

    std::cout << "pMemPool=" << pMemPool.get() << '\n';
    
    return 0;
}
