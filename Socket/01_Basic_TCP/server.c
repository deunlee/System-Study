#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE  4096
#define DEFAULT_IP   "0.0.0.0"
#define DEFAULT_PORT 55555

int main() {
    int sockfd, sockfd_accept;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buffer[BUFFER_SIZE] = { 0, };
    char buffer_send[] = "hello this is server!!";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("[Server] Cannot bind a socket.\n");
        return 1;
    }

    listen(sockfd, 1);
    printf("[Server] Server is running at %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    client_addr_size = sizeof(struct sockaddr);
    sockfd_accept = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
    if (sockfd_accept == -1) {
        printf("[Server] Failed accept a connection.\n");
        return 1;
    }

    printf("[Server] ListenFD=%d, AcceptFD=%d\n", sockfd, sockfd_accept);
    printf("[Server] ClientIP=%s, Port=%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    if (send(sockfd_accept, buffer_send, strlen(buffer_send) + 1, 0) == -1) {
        printf("[Server] Failed to send a message.\n");
        return 1;
    }

    if (recv(sockfd_accept, buffer, BUFFER_SIZE, 0) == -1) {
        printf("[Server] Failed to receive a message.\n");
        return 1;
    }

    printf("[Server] Received: %s\n", buffer);

    close(sockfd_accept);
    close(sockfd);

    return 0;
}