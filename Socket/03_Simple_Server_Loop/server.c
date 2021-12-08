#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS      1
#define BUFFER_SIZE   4096
#define DEFAULT_PORT 55555
#define DEFAULT_IP   "127.0.0.1"

int main() {
    int sockfd = 0, sockfd_accept = 0, option = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buffer[BUFFER_SIZE] = { 0, };

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP); // INADDR_ANY

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); // Address already in use 오류 무시
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot bind a socket");
        return 1;
    }

    listen(sockfd, MAX_CLIENTS);
    printf("[Server] Server is running at %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    while (1) {
        printf("[Server] Waiting for a client...\n");

        client_addr_size = sizeof(struct sockaddr);
        sockfd_accept = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
        if (sockfd_accept == -1) {
            perror("[Error] Failed accept a connection");
            close(sockfd);
            return 1;
        }

        printf("[Server] Client is connected. (%s:%d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // STEP 1. Say hello to client.
        strcpy(buffer, "Hello~ I'm Server!");
        if (send(sockfd_accept, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("[Error] Failed to send a message");
            close(sockfd);
            return 1;
        }

        // STEP 2. Receive message from client.
        if (recv(sockfd_accept, buffer, sizeof(BUFFER_SIZE), 0) == -1) {
            perror("[Error] Failed to receive a message");
            close(sockfd);
            return 1;
        }
        printf("[Server] Client says: %s\n\n", buffer);

        close(sockfd_accept);
    }

    close(sockfd);
    return 0;
}
