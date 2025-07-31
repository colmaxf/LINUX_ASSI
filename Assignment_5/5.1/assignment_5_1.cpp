#include <iostream>
#include <csignal>
#include <unistd.h>

// Global variable to count the number of times SIGINT signal is received
/*
volatile: Tells the compiler that the value of this variable can be changed by external factors (like a signal handler) at any time. 
          This prevents the compiler from making optimizations that could cause errors.

sig_atomic_t: An integer data type that ensures that read/write operations on it are "atomic", meaning that they cannot be interrupted by another signal.
*/
volatile sig_atomic_t sigint_count = 0;

/**
* @brief The function that handles the SIGINT signal.
* * @param signum The signal number (which will be SIGINT).
* * This function is called every time the program receives a SIGINT signal.
* It increments the counter, prints a message, and exits if it has received it three times.
*/
void handle_sigint(int signum) {
    sigint_count++;
    std::cout << "\nSIGINT received (" << sigint_count << "/3). Press Ctrl+C again to exit." << std::endl;
    
    if (sigint_count >= 3) {
        std::cout << "Exiting program." << std::endl;
        exit(0); // Ending program
    }
}

int main() {
    // 1. Register signal handler function
    // - signal(): Function of the <csignal> library.
    // - Parameter 1 (SIGINT): Signal to catch. SIGINT is an interrupt signal from the keyboard.
    // - Parameter 2 (handle_sigint): Pointer to the function that will be called when receiving the SIGINT signal.
    signal(SIGINT, handle_sigint);

    std::cout << "Program is running. Press Ctrl+C to send a SIGINT signal." << std::endl;
    std::cout << "The program will terminate after 3 signals." << std::endl;

    // 2. Infinite loop to keep the program running
    // - pause(): This function pauses the program until a signal is received.
    // This saves CPU compared to an empty while(true) loop.
    while (true) {
        pause();
    }

    return 0; 
}