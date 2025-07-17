#include <iostream>
#include <unistd.h>     // fork(), sleep(), getpid()
#include <sys/wait.h>   // wait()
#include <stdlib.h>     // exit()

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Lỗi
        perror("fork() failed");
        return 1;
    }

    if (pid == 0) {
        // --- CHILD Process Code ---
        std::cout << "[Child] My PID is " << getpid() << ". I will exit now." << std::endl;
        exit(0); // Child process exits immediately
    } else {
        // --- Code of CHA Process ---
        std::cout << "[Parent] My PID is" << getpid() << ". I created a child with PID " << pid << "." << std::endl;
        std::cout << "[Parent] I will sleep for 20 seconds and not call wait()..." << std::endl;
        
        // Sleep for a while to have time to observe the Zombie state
        sleep(20);

        // Parent does NOT call wait(), making son a Zombie
        std::cout << "[Parent] I woke up and will escape." << std::endl;
        
        // Even though the parent program exits, during the 20 second sleep, the child process is a Zombie.
        // wait(NULL); // This line, if added, will clean up the Zombie
    }

    return 0;
}