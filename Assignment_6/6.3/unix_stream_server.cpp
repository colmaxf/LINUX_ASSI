#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

void die(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

int main() {
    const char* SOCKET_PATH = "/tmp/unix_stream.sock";
    char buffer[1024] = {0};

    // 1. Create a socket
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        die("socket failed");
    }

    // 2. Set up the address (file path)
    sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Remove the old socket file if it exists
    unlink(SOCKET_PATH);

    // 3. Bind the socket to the file path
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        die("bind failed");
    }

    // 4. Listen for connections
    if (listen(server_fd, 3) < 0) {
        die("listen failed");
    }
    std::cout << "Server is listening on " << SOCKET_PATH << std::endl;

    // 5. Accept a connection
    int new_socket = accept(server_fd, NULL, NULL);
    if (new_socket < 0) {
        die("accept failed");
    }

    // 6. Read and send data
    read(new_socket, buffer, 1024);
    std::cout << "Client says: " << buffer << std::endl;
    const char* response = "Hello from Unix Stream Server";
    send(new_socket, response, strlen(response), 0);

    // 7. Clean up
    close(new_socket);
    close(server_fd);
    unlink(SOCKET_PATH); // Remove the socket file when done

    return 0;
}