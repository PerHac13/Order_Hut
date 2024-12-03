
#include "order_manager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

OrderManager::OrderManager() = default;

void OrderManager::run() {
    // Start supervisor and processor threads
    std::thread supervisor_thread(&OrderManager::supervisor, this);
    std::thread processor_thread(&OrderManager::processCommands, this);

    // Wait for existing read threads to complete
    for (auto& thread : read_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    read_threads.clear();

    // Wait for threads to complete
    supervisor_thread.join();
    processor_thread.join();
}

void OrderManager::printTimestampedMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(print_mutex);
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&now_c), "%T") << " " << message << std::endl;
}

void OrderManager::supervisor() {
    std::string command;

    while (is_running) {
        std::getline(std::cin, command);
        
        if (command == "Q") {
            is_running = false;
            break;
        }

        {
            std::lock_guard<std::mutex> lock(command_mutex);
            command_queue.push(command);
        }
        command_cv.notify_one();
    }

    // Notify processor to exit
    command_cv.notify_one();
}

void OrderManager::processCommands() {
    int reader_number = 0;

    while (is_running) {
        std::string command;
        {
            std::unique_lock<std::mutex> lock(command_mutex);
            command_cv.wait(lock, [this]() { 
                return !command_queue.empty() || !is_running; 
            });
            
            if (!is_running && command_queue.empty()) {
                break;
            }

            command = command_queue.front();
            command_queue.pop();
        }

        if (command == "Q") {
            break;
        }

        if (command[0] == 'R') {
            int read_order = std::stoi(command.substr(1));
            // Create a new thread for read operation
            read_threads.emplace_back(&OrderManager::executeRead, 
                                      this, read_order, ++reader_number);
        } else if (command == "W") {
            executeWrite();
        }
    }

    is_running = false;
}

void OrderManager::executeRead(int read_order, int reader_number) {
    // Wait in service queue
    service_queue_lock.acquire();

    // Acquire read lock
    read_lock.acquire();
    reader_count++;
    
    // If first reader, block writers
    if (reader_count == 1) {
        write_lock.acquire();
    }

    // Release read lock and service queue
    read_lock.release();
    service_queue_lock.release();

    // Simulate reading
    printTimestampedMessage("Reader " + std::to_string(reader_number) + 
                            " is reading order " + std::to_string(read_order));
    
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Check if order is prepared
    bool isPrepared = read_order <= latest_order;
    
    printTimestampedMessage("Reader " + std::to_string(reader_number) + 
                            " has read order " + std::to_string(read_order) + 
                            " and it is " + (isPrepared ? "prepared" : "not prepared"));

    // Acquire read lock
    read_lock.acquire();
    reader_count--;

    // If last reader, release writers
    if (reader_count == 0) {
        write_lock.release();
    }

    // Release read lock
    read_lock.release();
}

void OrderManager::executeWrite() {
    // Wait in service queue
    service_queue_lock.acquire();

    // Acquire write lock
    write_lock.acquire();

    // Simulate writing
    printTimestampedMessage("Starting write operation");
    
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Increment latest order
    latest_order++;
    
    printTimestampedMessage("Write complete. Current order number: " + 
                            std::to_string(latest_order));

    // Release write lock and service queue
    write_lock.release();
    service_queue_lock.release();
}