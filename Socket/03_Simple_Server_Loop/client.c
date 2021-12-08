#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE   4096
#define DEFAULT_PORT 55555
#define DEFAULT_IP   "127.0.0.1"

int main() {
    int i, sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0, };

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot connect to the server");
        return 1;
    }
    printf("[Client] Connected to the server. (%s:%d)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    // STEP 1. Receive message from server.
    if (recv(sockfd, buffer, sizeof(BUFFER_SIZE), 0) == -1) {
        perror("[Error] Failed to receive a message");
        close(sockfd);
        return 1;
    }
    printf("[Client] Server says: %s\n", buffer);

    // STEP 2. Say hi to server.
    strcpy(buffer, "Hi! I'm Client!!");
    if (send(sockfd, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("[Error] Failed to send a message");
        close(sockfd);
        return 1;
    }

    printf("[Client] Message has been sent to server.\n");
    close(sockfd);
    return 0;    
}
