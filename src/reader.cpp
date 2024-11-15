#include "reader.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <mutex>

std::mutex coutMutex;

Reader::Reader(OrderCounter& counter, int readerNumber)
    : counter(counter), readerNumber(readerNumber) {}

void Reader::queryOrder(int orderNumber) {

    sem_wait(counter.getReadLock());
    counter.getReaderCount()++;
    if (counter.getReaderCount() == 1) {
        sem_wait(counter.getWriteLock());
    }
    sem_post(counter.getReadLock());

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&now_c), "%T") << " Reader " << readerNumber << " is reading order " << orderNumber << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    bool isPrepared = orderNumber <= counter.getLatestOrder();
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&now_c), "%T") << " Reader " << readerNumber << " has read order " << orderNumber << " and it is " << (isPrepared ? "prepared" : "not prepared") << std::endl;
    }

    sem_wait(counter.getReadLock());
    counter.getReaderCount()--;
    if(counter.getReaderCount() == 0) {
        sem_post(counter.getWriteLock());
    }
    sem_post(counter.getReadLock());
}