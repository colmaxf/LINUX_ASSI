#include <iostream>
#include <cstdlib>      // For setenv, getenv
#include <unistd.h>     // For fork, execlp
#include <sys/wait.h>   // For wait

int main() {
    // 1. Parent process sets environment variable
    // Syntax: setenv("VARIABLE_NAME", "VALUE", 1);
    // The third parameter is 1 to overwrite if the variable already exists.
    if (setenv("MY_COMMAND", "ls", 1) != 0) {
        perror("setenv() failed");
        return EXIT_FAILURE;
    }

    std::cout << "[Parent] Environment variables set MY_COMMAND=ls" << std::endl;

    // Create child process
    pid_t pid = fork();

    if (pid < 0) {
        // Error when fork()
        perror("fork() failed");
        return EXIT_FAILURE;

    } else if (pid == 0) {
        // *** This is the code of the CHILD PROCESS ***
        std::cout << "[Child] Child process running..." << std::endl;

        // 2. Child process reads environment variable
        const char* command = getenv("MY_COMMAND");

        if (command == NULL) {
            std::cerr << "[Child] Error: environment variable MY_COMMAND not found" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "[Child] Command: " << command << ". Prepare for execution..." << std::endl;
        std::cout << "-----------------------------------------" << std::endl;


        // 3. The child process uses execlp() to replace the command code
        // execlp will search for the 'command' command in the directories of the PATH variable
        // The second and subsequent parameters are arguments to the command
        // Must end with a NULL pointer
        execlp(command, command, "-l", "-a", (char*)NULL);

        // IMPORTANT: If execlp() succeeds, the lines of code below will NEVER run
        // because the entire child process's code has been replaced.
        // If it runs to this point, it means there was an error.
        perror("execlp() failed");
        exit(EXIT_FAILURE); // Exit the child process if exec fails

    } else {
        // *** This is the code of the PARENT PROCESS ***
        std::cout << "[Parent] Created child process with PID: " << pid << std::endl;
        
        // Wait for child process to finish to avoid "zombie" process
        wait(NULL);
        
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "[Parent] Child process terminated. Parent process exited." << std::endl;
    }

    return EXIT_SUCCESS;
}