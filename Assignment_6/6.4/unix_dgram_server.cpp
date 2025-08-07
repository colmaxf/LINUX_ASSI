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
    char buffer[1024];

    // 1. Create socket
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        die("socket failed");
    }

    sockaddr_un servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strncpy(servaddr.sun_path, SERVER_PATH, sizeof(servaddr.sun_path) - 1);

    unlink(SERVER_PATH);

    // 2. Bind the socket
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        die("bind failed");
    }
    std::cout << "Server is listening at " << SERVER_PATH << std::endl;

    socklen_t len = sizeof(cliaddr);

    // 3. Receive data
    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&cliaddr, &len);
    if (n < 0) {
        die("recvfrom failed");
    }
    buffer[n] = '\0';
    std::cout << "Client says: " << buffer << std::endl;

    // 4. Send response
    const char* response = "Hello from Unix Dgram Server";
    sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&cliaddr, len);

    // 5. Clean up
    close(sockfd);
    unlink(SERVER_PATH);

    return 0;
}