#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/select.h>
#include <vector>

// Global flags to indicate that a signal has been received.
volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigterm_received = 0;

/**
 * @brief General handler for signals.
 * This function only sets a flag. The main processing logic will be in the select() loop.
 */
void signal_handler(int signum) {
    if (signum == SIGINT) {
        sigint_received = 1;
    } else if (signum == SIGTERM) {
        sigterm_received = 1;
    }
}

int main() {
    // Use sigaction for more reliable signal handling.
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // No SA_RESTART, so select() is interrupted.
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    std::cout << "Program is running. Waiting for input or signals (SIGINT/SIGTERM)." << std::endl;
    std::cout << "To send SIGINT, press Ctrl+C." << std::endl;
    std::cout << "To send SIGTERM, use 'kill " << getpid() << "' from another terminal." << std::endl;

    // Prepare signal masks to safely wait for signals.
    sigset_t blocked_mask, empty_mask;
    sigemptyset(&blocked_mask);
    sigaddset(&blocked_mask, SIGINT);
    sigaddset(&blocked_mask, SIGTERM);
    sigemptyset(&empty_mask);

    // Block the signals in the main thread. They will be unblocked only during pselect().
    if (sigprocmask(SIG_BLOCK, &blocked_mask, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    while (true) {
        // Check the signal flags before entering select().
        if (sigint_received) {
            std::cout << "\nSIGINT received." << std::endl;
            sigint_received = 0; // Reset the flag.
        }
        if (sigterm_received) {
            std::cout << "\nSIGTERM received. Exiting." << std::endl;
            break; // Exit the loop.
        }

        // 1. Prepare the set of file descriptors to monitor.
        fd_set read_fds;
        FD_ZERO(&read_fds); // Clear the set.
        FD_SET(STDIN_FILENO, &read_fds); // Add stdin (the keyboard) to the set.

        // 2. Call pselect() to wait safely.
        // - pselect() will atomically unblock the signals in 'empty_mask'
        //   (i.e., it unblocks SIGINT/SIGTERM), wait for I/O or a signal,
        //   and then re-block them before returning. This avoids race conditions.
        // - It will block until:
        //   a) There is data to read on one of the file descriptors (here, stdin).
        //   b) A signal interrupts it.
        // - Parameter 1 (STDIN_FILENO + 1): The highest-numbered file descriptor to check + 1.
        // - Parameter 2 (&read_fds): The set of fds to monitor for reading.
        // - The last parameter (&empty_mask): The signal mask to apply during the wait.
        int activity = pselect(STDIN_FILENO + 1, &read_fds, NULL, NULL, NULL, &empty_mask);

        if (activity < 0) {
            // When a signal is caught, pselect() returns -1 and sets errno to EINTR.
            // The loop will continue, and the signal flags will be checked on the next iteration.
            continue; 
        }

        // 3. Check if stdin is ready for reading.
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char buffer[256];
            // Read data from the keyboard.
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Add a null terminator to the string.
                std::cout << "You entered: " << buffer;
            }
        }
    }

    return 0;
}