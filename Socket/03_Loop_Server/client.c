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

int main(int argc, char* argv[]) {
    int sockfd, is_slow_client = 0;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0, };

    // To test the server, act as a slow client if -s option is given.
    if (argc >= 2 && strcmp(argv[1], "-s") == 0) {
        is_slow_client = 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot connect to the server");
        return 1;
    }
    printf("[Client] (%d) Connected to the server. (%s:%d)\n", getpid(), inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    // STEP 1. Receive message from the server.
    if (recv(sockfd, buffer, BUFFER_SIZE, 0) == -1) {
        perror("[Error] Failed to receive a message");
        close(sockfd);
        return 1;
    }
    printf("[Client] (%d) Server says: %s\n", getpid(), buffer);

    // STEP 2. Say hi to the server.
    if (is_slow_client) { // Slow client simulation
        printf("[Client] (%d) I'm going to sleep...\n", getpid());
        sleep(10);
    }
    sprintf(buffer, "Hi! I'm client and my pid is %d.", getpid());
    if (send(sockfd, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("[Error] Failed to send a message");
        close(sockfd);
        return 1;
    }
    printf("[Client] (%d) Sent a message to the server.\n", getpid());

    close(sockfd);
    return 0;
}
