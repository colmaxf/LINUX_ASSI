#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Shared resources
std::mutex mtx;
std::condition_variable cv;
int data = 0;
bool ready = false; // Flag to indicate if data is ready
bool processed = false; // Flag to indicate if data has been processed

void producer() {
    for (int i = 0; i < 10; ++i) {
        {
            // Use unique_lock because it can be unlocked manually
            std::unique_lock<std::mutex> lock(mtx);
            
            // Wait until the consumer has processed the old data
            cv.wait(lock, []{ return processed; });

            data = rand() % 100;
            ready = true;
            processed = false;
            std::cout << "Producer produced: " << data << std::endl;
        } // Old lock is unlocked here
        
        cv.notify_one(); // Notify the consumer
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void consumer() {
    // Initialize the state so the producer can run for the first time
    {
        std::lock_guard<std::mutex> lock(mtx);
        processed = true;
    }
    cv.notify_one();

    for (int i = 0; i < 10; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait until the producer has finished producing
        // wait will automatically unlock the mutex and put the thread to sleep.
        // When notified, it wakes up, re-acquires the lock, and checks the condition (the lambda).
        // If the condition is false (spurious wakeup), it goes back to waiting.
        cv.wait(lock, []{ return ready; });

        std::cout << "Consumer consumed: " << data << std::endl << std::endl;
        ready = false;
        processed = true;
        
        // Unlock before notifying so the producer can acquire the lock immediately
        lock.unlock(); 
        cv.notify_one(); // Notify the producer that processing is done
    }
}

int main() {
    srand(time(0));
    std::cout << "Starting Producer-Consumer model.\n";

    std::thread p(producer);
    std::thread c(consumer);

    p.join();
    c.join();

    std::cout << "Producer-Consumer model finished.\n";
    return 0;
}