gcc  -O0 threadNonSyncTest.c -o threadNonSyncTest && ./threadNonSyncTest

g++ -std=gnu++11  -O2 threadPosixLockPerfTest.cpp PoolLock.cpp -o threadPosixLockPerfTest && ./threadPosixLockPerfTest
