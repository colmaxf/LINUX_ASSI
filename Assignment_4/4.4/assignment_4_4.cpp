#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <shared_mutex> // C++17
#include <chrono>

#define NUM_READERS 5
#define NUM_WRITERS 2

// shared resource
int shared_resource = 100;

// shared_mutex
std::shared_mutex rw_lock;

/**
* @brief Reader function
* @param int id
 */
void reader_function(int id) {
    while (true) {
    
        {    // shared lock
            std::shared_lock<std::shared_mutex> lock(rw_lock);

            // Multiple reading threads can read at the same time
            std::cout << "   Reader " << id << " reads value = " << shared_resource << std::endl;

            //The lock is automatically released when 'lock' goes out of scope.
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        // Wait for a bit before trying to read again.
        // This is the key fix to prevent writer starvation, creating a window
        // where the lock is free for writers to acquire.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));        
    }
}

/**
* @brief Writer function
* @param int id
 */
void writer_function(int id) {
    while (true) {
        {    // exclusive lock
            std::unique_lock<std::shared_mutex> lock(rw_lock);

            // Only one thread can write to this at a time.
            shared_resource++;
            std::cout << "Writer " << id << " writes new value i = " << shared_resource << " <<<<<<<<<<<<<<" << std::endl;

            // Simulate time spent writing
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }    
        // The lock is automatically released when 'lock' goes out of scope.
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    std::vector<std::thread> threads;

    std::cout << "Read-Write Lock: Observe the output to see the Readers running in parallel.\n";

    // Create Reader thread
    for (int i = 0; i < NUM_WRITERS; ++i) {
        threads.push_back(std::thread(writer_function, i + 1));
    }

    // Create Writer Thread
    for (int i = 0; i < NUM_READERS; ++i) {
        threads.push_back(std::thread(reader_function, i + 1));
    }

    // Wait for all threads 
    // Program runs forever, Ctrl+C to stop
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}