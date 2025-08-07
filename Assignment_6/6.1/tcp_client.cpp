#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void die(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

int main() {
    const int PORT = 8080;
    const char* SERVER_IP = "127.0.0.1"; // Localhost
    char buffer[1024] = {0};
    const char *hello_message = "Hello from Client";

    // 1. Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        die("socket creation error");
    }

    // 2. Set up the Server's address
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        die("Invalid address/ Address not supported");
    }

    // 3. Connect to the Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        die("Connection Failed");
    }
    std::cout << "Connected to the server!" << std::endl;

    // 4. Send data to the Server
    ssize_t bytes_sent = send(sock, hello_message, strlen(hello_message), 0);
    if (bytes_sent == -1) {
        die("send failed");
    }
    std::cout << "Message sent to the server." << std::endl;

    // 5. Receive response from the Server
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Ensure the buffer is null-terminated
        std::cout << "Server response: " << buffer << std::endl;
    } else if (bytes_read == 0) {
        std::cout << "Server closed the connection." << std::endl;
    } else {
        die("read failed");
    }

    // 6. Close the connection
    close(sock);

    return 0;
}