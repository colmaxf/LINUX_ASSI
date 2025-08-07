#include <iostream>
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
    const char* SERVER_IP = "127.0.0.1";
    char buffer[1024];
    const char *message = "Hello from UDP Client";

    // 1. Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        die("socket creation failed");
    }

    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    // 2. Set up server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    int n;
    socklen_t len = sizeof(servaddr);

    // 3. Send message to server
    sendto(sockfd, (const char *)message, strlen(message), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    std::cout << "Message sent to server." << std::endl;

    // 4. Receive response from server
    n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';
    std::cout << "Server response: " << buffer << std::endl;

    close(sockfd);
    return 0;
}