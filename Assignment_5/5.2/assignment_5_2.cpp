#include <iostream>
#include <csignal>
#include <unistd.h>

// Variable to count the number of seconds passed
/*
volatile: Tells the compiler that the value of this variable can be changed by external factors (like a signal handler) at any time. 
          This prevents the compiler from making optimizations that could cause errors.

sig_atomic_t: An integer data type that ensures that read/write operations on it are "atomic", meaning that they cannot be interrupted by another signal.
*/
volatile sig_atomic_t timer_count = 0;

/**
* @brief The SIGALRM signal handler function.
* * @param signum The signal number (will be SIGALRM).
* * This function increments the counter, prints the time and resets the alarm.
* If the count reaches 10 seconds, the program exits.
*/
void handle_sigalrm(int signum) {
    timer_count++;
    std::cout << "Timer: " << timer_count << " seconds" << std::endl;

    if (timer_count >= 10) {
        std::cout << "10 seconds have passed. Exiting." << std::endl;
        exit(0);
    }

    // Reset the alarm to go off in 1 second
    alarm(1);
}

int main() {
    // Register a handler function for the SIGALRM signal
    signal(SIGALRM, handle_sigalrm);

    std::cout << "Starting a 10-second timer..." << std::endl;

    // 1. Set the initial alarm.
    // - alarm(1): Requests the OS to send a SIGALRM signal to this program after 1 second.
    alarm(1);

    // 2. Keep the program running to receive signals.
    while (true) {
        pause();
    }

    return 0;
}