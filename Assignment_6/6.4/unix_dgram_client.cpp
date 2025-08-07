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
    const char* SERVER_PATH = "/tmp/unix_dgram_server.sock";
    const char* CLIENT_PATH = "/tmp/unix_dgram_client.sock"; // The client needs to bind so the server knows where to send the response
    char buffer[1024];

    // 1. Create socket
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        die("socket failed");
    }

    // 2. Bind the client socket to its own path
    sockaddr_un cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strncpy(cliaddr.sun_path, CLIENT_PATH, sizeof(cliaddr.sun_path) - 1);
    unlink(CLIENT_PATH);
    if (bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
        die("client bind failed");
    }

    // 3. Set up the server address
    sockaddr_un servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strncpy(servaddr.sun_path, SERVER_PATH, sizeof(servaddr.sun_path) - 1);

    // 4. Send data
    const char* message = "Hello from Unix Dgram Client";
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // 5. Receive data
    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    if (n < 0) {
        die("recvfrom failed");
    }
    buffer[n] = '\0';
    std::cout << "Server response: " << buffer << std::endl;

    // 6. Clean up
    close(sockfd);
    unlink(CLIENT_PATH);

    return 0;
}