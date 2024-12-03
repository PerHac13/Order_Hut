#pragma once

#include "order_counter.h"

class Writer {
    public:
        Writer(OrderCounter& counter);
        void prepareOrder();
    
    private:
        OrderCounter& counter;  
};