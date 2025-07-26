#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// global variable
long long counter = 0;

// global mmutex
std::mutex counter_mutex;

/**
* @brief Increase count variable
* @param void
 */
void increment_function() {
    for (int i = 0; i < 1000000; ++i) {
        // std::lock_guard is a RAII mechanism.
        // Mutex is automatically locked when 'guard' is created.
        std::lock_guard<std::mutex> guard(counter_mutex);

        // Increasing Counter is safe
        counter++;

    } // Mutex is automatically unlocked when 'guard' goes out of scope (destroyed).
}

int main() {
    std::vector<std::thread> threads;

    std::cout << "Increasing \"counter\" with 3 threads..." << std::endl;

    // Creating 3 thread
    for (int i = 0; i < 3; ++i) {
        threads.push_back(std::thread(increment_function));
    }

    // Waiting for ending 3 thread
    for (auto& th : threads) {
        th.join();
    }


    std::cout << " The final value of counter variable is: " << counter << std::endl;
    std::cout << " The exoect value is: 3000000" << std::endl;

    return 0;
}
