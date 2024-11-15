#pragma once

#include "order_counter.h"

class Reader {
    public:
        Reader(OrderCounter& counter, int readerNumber);
        void queryOrder(int orderNumber);

    private:
        OrderCounter& counter;
        int readerNumber;  
};