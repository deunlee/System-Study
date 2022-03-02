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
#define DEFAULT_IP   "0.0.0.0"

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buffer_recv[BUFFER_SIZE] = { 0, };
    char buffer_send[] = "Hello~ This is the server!!";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM == UDP

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot bind a socket");
        return 1;
    }

    client_addr_size = sizeof(client_addr);
    if (recvfrom(sockfd, buffer_recv, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_size) == -1) {
        perror("[Error] Failed to receive a message");
        return 1;
    }
    printf("[Server] Received: %s\n", buffer_recv);
    printf("[Server] Client Info: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    client_addr_size = sizeof(client_addr);
    if (sendto(sockfd, buffer_send, strlen(buffer_send) + 1, 0, (struct sockaddr*)&client_addr, client_addr_size) == -1) {
        perror("[Error] Failed to send a message");
        return 1;
    }
    printf("[Server] Sent a message to the client.\n");

    close(sockfd);
    return 0;
}
