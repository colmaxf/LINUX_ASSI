#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h> // For signal handling

// Simple error handling function
void die(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

int main() {
    const int PORT = 8080;
    char buffer[1024] = {0};
    const char *hello_message = "Hello from Server";

    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        die("socket failed");
    }

    // Allow reusing the address to avoid "Address already in use" errors
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        die("setsockopt");
    }

    // 2. Assign an address (IP and port) to the socket
    sockaddr_in address;
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(PORT); // Convert port to network byte order

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        die("bind failed");
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) < 0) { // Maximum queue of 3 connections
        die("listen failed");
    }
    std::cout << "Server is listening on port " << PORT << std::endl;

    // Prevent zombie processes by having the kernel auto-reap terminated children
    signal(SIGCHLD, SIG_IGN);

    while (true) {
        // 4. Accept a connection
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept failed"); // Use perror to not exit the server on a failed accept
            continue; // Continue to the next iteration to accept other clients
        }

        // Create a child process to handle the new client
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            close(new_socket);
            continue;
        }

        if (pid == 0) { // This is the child process
            close(server_fd); // Child doesn't need the listener socket
            std::cout << "Client connected! Handling in process " << getpid() << std::endl;

            // 5. Receive data from the Client
            ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Ensure null-termination for safety
                std::cout << "[PID " << getpid() << "] Client says: " << buffer << std::endl;

                // 6. Send a response to the Client
                if (send(new_socket, hello_message, strlen(hello_message), 0) == -1) {
                    perror("send failed");
                } else {
                    std::cout << "[PID " << getpid() << "] Response sent to the client." << std::endl;
                }
            } else if (bytes_read == 0) {
                std::cout << "[PID " << getpid() << "] Client closed the connection." << std::endl;
            } else {
                perror("read failed");
            }
            close(new_socket); // Close the client socket
            exit(0); // Terminate the child process
        } else { // This is the parent process
            close(new_socket); // Parent doesn't need the connected socket
        }
    }

    // This part is unreachable in the current infinite loop, but good practice
    close(server_fd);

    return 0;
}