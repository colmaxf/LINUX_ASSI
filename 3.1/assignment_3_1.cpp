#include <iostream>   
#include <string>     
#include <cstdlib>    // EXIT_SUCCESS, EXIT_FAILURE, exit
#include <unistd.h>   // fork, getpid, getppid, sleep
#include <sys/wait.h> // wait, WIFEXITED, WEXITSTATUS

int main() {
    pid_t pid; // /pid of process

    std::cout << "--- Exercise 1: Initializing and Cleaning Up Processes ---" << std::endl;
    std::cout << "Parent process (PID: " << getpid() << ") is starting." << std::endl;

    pid = fork(); // create child process

    if (pid < 0) {
        // fail
        std::cerr << "Error: fork() fail." << std::endl;
        return EXIT_FAILURE;
    } else if (pid == 0) {
        //------------------Child Process---------------------------------
        std::cout << "Child process (PID: " << getpid() << ") is create. PPID of child is " << getppid() << "." << std::endl;
        std::cout << "The child process is doing work and will exit with code 10." << std::endl;
        sleep(1); 
        exit(10); 
        //---------------------------------------------------
    } else {
        //------------------Parent Process-------------------------------
        int status; // status of child

        std::cout << "Parent process (PID: " << getpid() << ") created child process with PID: " << pid << "." << std::endl;
        std::cout << "Waiting for the child to finish..." << std::endl;

        // Waiting for the child to finish
        if (wait(&status) == -1) {
            std::cerr << "Error: wait() fail." << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Cleaned up the child process with PID: " << pid << "." << std::endl;

        // Check if the child process exits normally
        if (WIFEXITED(status)) {
            std::cout << "The child process exited normally with the code: " << WEXITSTATUS(status) << "." << std::endl;
        } else {
            std::cout << "Child process does not exit normally." << std::endl;
        }
    }

    std::cout << "Finishing Parent Process." << std::endl;
    return EXIT_SUCCESS;
}