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
    char buffer[1024];

    // 1. Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        die("socket creation failed");
    }

    sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // 2. Set up server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // 3. Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        die("bind failed");
    }
    std::cout << "UDP Server listening on port " << PORT << std::endl;

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n;

        // 4. Receive message from client
        std::cout << "Waiting for message..." << std::endl;
        n = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *) &cliaddr, &len);
        if (n < 0) {
            die("recvfrom failed");
        }
        buffer[n] = '\0'; // Null-terminate the string
        std::cout << "Client from " << inet_ntoa(cliaddr.sin_addr) << ":" << ntohs(cliaddr.sin_port) << " says: " << buffer << std::endl;

        // 5. Send the same message back to client (echo)
        sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *) &cliaddr, len);
        std::cout << "Echo message sent." << std::endl;
    }

    close(sockfd);
    return 0;
}