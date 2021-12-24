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
    socklen_t server_addr_size;
    char buffer_recv[BUFFER_SIZE] = { 0, };
    char buffer_send[] = "Hello from the client~";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM == UDP

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    server_addr_size = sizeof(server_addr);
    if (sendto(sockfd, buffer_send, strlen(buffer_send) + 1, 0, (struct sockaddr*)&server_addr, server_addr_size) == -1) {
        perror("[Error] Failed to send a message");
        return 1;
    }
    printf("[Client] Sent a message to the server.\n");

    if (recvfrom(sockfd, buffer_recv, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_addr_size) == -1) {
        perror("[Error] Failed to receive a message");
        return 1;
    }
    printf("[Client] Received: %s\n", buffer_recv);

    close(sockfd);
    return 0;
}
