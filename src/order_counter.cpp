#include "order_counter.h"

OrderCounter::OrderCounter() : latestOrder(0), readerCount(0) {
    sem_init(&readLock, 0, 1);
    sem_init(&writeLock, 0, 1);
}

OrderCounter::~OrderCounter() {
    sem_destroy(&readLock);
    sem_destroy(&writeLock);
}

int OrderCounter::getLatestOrder() const {
    return latestOrder.load();
}

void OrderCounter::incrementOrder() {
    latestOrder++;
}