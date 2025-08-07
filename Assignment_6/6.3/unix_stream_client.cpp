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
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        die("socket failed");
    }

    // 2. Set up the server address
    sockaddr_un serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

    // 3. Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        die("connect failed");
    }

    // 4. Send and receive data
    const char* message = "Hello from Unix Stream Client";
    send(sock, message, strlen(message), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;

    // 5. Close the connection
    close(sock);

    return 0;
}