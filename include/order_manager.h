#ifndef ORDER_MANAGER_H
#define __ORDER_MANAGER_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <string>
#include <thread>
#include <semaphore>
#include <vector>

class OrderManager {
public:
    OrderManager();
    void run();

private:
    // Semaphores and mutexes
    std::binary_semaphore service_queue_lock{1};
    std::binary_semaphore read_lock{1};
    std::binary_semaphore write_lock{1};

    // Atomic variables
    std::atomic<int> reader_count{0};
    std::atomic<int> latest_order{0};
    std::atomic<bool> is_running{true};

    // Command queue management
    std::mutex command_mutex;
    std::condition_variable command_cv;
    std::queue<std::string> command_queue;

    // Threads for parallel reads
    std::vector<std::thread> read_threads;

    // Mutex for thread-safe printing
    std::mutex print_mutex;

    void supervisor();
    void processCommands();
    void executeRead(int read_order, int reader_number);
    void executeWrite();
    void printTimestampedMessage(const std::string& message);
};

#endif // ORDER_MANAGER_H