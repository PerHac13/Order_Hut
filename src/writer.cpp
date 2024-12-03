#include "writer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <mutex>

extern std::mutex coutMutex;

Writer::Writer(OrderCounter& counter) : counter(counter) {}

void Writer::prepareOrder() {
    sem_wait(counter.getServiceQueueLock());
    sem_wait(counter.getWriteLock());
    sem_wait(counter.getServiceQueueLock());

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&now_c), "%T") << " Writer is preparing order " << counter.getLatestOrder() + 1 << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(15));

    counter.incrementOrder();
    
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&now_c), "%T") << " Writer has prepared order " << counter.getLatestOrder() << std::endl;
    }

    sem_post(counter.getWriteLock());
}