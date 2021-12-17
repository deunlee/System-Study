#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE   1024
#define DEFAULT_PORT 55555
#define DEFAULT_IP   "127.0.0.1"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer_recv[BUFFER_SIZE] = { 0, };
    char buffer_send[] = "hello from client~";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("[Client] Cannot connect to the server.\n");
        return 1;
    }
    printf("[Client] Connected to %s:%d (sockfd=%d)\n",
            inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), sockfd);

    if (recv(sockfd, buffer_recv, BUFFER_SIZE, 0) == -1) {
        printf("[Client] Failed to receive a message.\n");
        return 1;
    }
    printf("[Client] Received: %s\n", buffer_recv);

    if (send(sockfd, buffer_send, strlen(buffer_send) + 1, 0) == -1) {
        printf("[Client] Failed to send a message.\n");
        return 1;
    }

    close(sockfd);
    return 0;
}
