#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

// Counter for the number of signals received.
volatile sig_atomic_t signal_received_count = 0;

/**
 * @brief Handles the SIGUSR1 signal in the child process.
 */
void handle_sigusr1(int signum) {
    std::cout << "Child: Received signal from parent." << std::endl;
    signal_received_count++;
}

int main() {
    // 1. Create a child process using fork().
    // - fork() returns the child's PID to the parent process.
    // - fork() returns 0 to the child process.
    // - fork() returns -1 on error.
    pid_t pid = fork();

    if (pid < 0) {
        // Case where fork() fails.
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // 2. This is the code for the CHILD PROCESS.
        std::cout << "Child process created with PID: " << getpid() << std::endl;
        
        // The child registers the handler for the SIGUSR1 signal.
        signal(SIGUSR1, handle_sigusr1);

        // The child waits for signals from the parent.
        while (signal_received_count < 5) {
            pause(); // Wait until a signal is received.
        }
        std::cout << "Child: Received 5 signals. Exiting." << std::endl;
    } else {
        // 3. This is the code for the PARENT PROCESS.
        std::cout << "Parent process created child with PID: " << pid << std::endl;

        for (int i = 0; i < 5; ++i) {
            // Wait for 2 seconds.
            sleep(2);
            std::cout << "Parent: Sending SIGUSR1 to child..." << std::endl;
            
            // Send the SIGUSR1 signal to the child process.
            // - kill(): Function to send a signal to a specific process.
            // - Parameter 1 (pid): PID of the receiving prsocess (the child).
            // - Parameter 2 (SIGUSR1): The signal to send.
            kill(pid, SIGUSR1);
        }

        // Wait for the child process to terminate.
        wait(NULL);
        std::cout << "Parent: Child process has finished. Exiting." << std::endl;
    }

    return 0;
}