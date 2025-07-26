#include <iostream>
#include <thread>
#include <chrono>

/**
* @brief Function will be processing by thread
* @param int nunmber of thread
 */
void thread_function(int thread_num) {
    // Get Id of curent thread
    std::thread::id thread_id = std::this_thread::get_id();

    std::cout << "Thread " << thread_num  << " running" << " has ID " << thread_id << std::endl;
    
    // thread is working
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << " Thread " << thread_num <<  " completing " << " has ID " << thread_id << std::endl;
}

int main() {
    std::cout << "Main Thread: Creating 2 children Thread." << std::endl;

    // Creating and Running First thread
    // Parameter 1: The function that the thread will execute
    // Next parameters: The arguments for that function
    std::thread t1(thread_function, 1);

    // Creating and Running second thread
    std::thread t2(thread_function, 2);

    std::cout << "Main Thread: Completed 2 Children Thread successfully. Waiting for the end of them..." << std::endl;

    // Waiting for ending first thread
    // Main thread will wait t1 until t1 completes.
    t1.join();
    std::cout << "Main Thread: First Thread completed." << std::endl;

    // Waiting for ending second thread
    t2.join();
    std::cout << "Main Thread: Second Thread completed." << std::endl;

    std::cout << "Main Thread: All off thread completed. Exit program..." << std::endl;

    return 0;
}
