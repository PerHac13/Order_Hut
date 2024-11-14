#pragma once

#include <semaphore>
#include <atomic>


#include <semaphore.h>
#include <atomic>

class OrderCounter {
    public:
        OrderCounter();
        ~OrderCounter();
    
        int getLatestOrder() const;
        void incrementOrder();
    
        sem_t* getReadLock() { return &readLock; }
        sem_t* getWriteLock() { return &writeLock; }
        std::atomic<int>& getReaderCount() { return readerCount; }

    private:
        std::atomic<int> latestOrder;
        std::atomic<int> readerCount;
        sem_t readLock;
        sem_t writeLock;
};