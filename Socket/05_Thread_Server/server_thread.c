#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS          5 // backlog
#define BUFFER_SIZE       1024
#define DEFAULT_PORT     55555
#define DEFAULT_IP   "0.0.0.0"

struct thread_info {
    pthread_t tid;
    int sockfd;
};

int communicate(int sockfd) {
    char* buffer = malloc(BUFFER_SIZE);

    // STEP 1. Say hello to the client.
    strcpy(buffer, "Hello~ I'm a server!");
    if (send(sockfd, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("[Error] Failed to send a message");
        close(sockfd);
        free(buffer);
        return 1;
    }

    // STEP 2. Receive message from the client.
    if (recv(sockfd, buffer, BUFFER_SIZE, 0) == -1) {
        perror("[Error] Failed to receive a message");
        close(sockfd);
        free(buffer);
        return 1;
    }
    printf("[Thread] (TID=%ld) Client says: %s\n", pthread_self(), buffer);

    close(sockfd);
    free(buffer);
    return 0;
}

void* thread(void* arg) {
    struct thread_info* info = (struct thread_info*)arg;
    communicate(info->sockfd);
    free(info);
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    int sockfd, sockfd_accept, option = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    struct thread_info* info;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    // Enable socket reuse. (Ignore address already in use error)
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot bind a socket");
        return 1;
    }

    listen(sockfd, MAX_CLIENTS);
    printf("[Server] Server is running at %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    while (1) {
        client_addr_size = sizeof(client_addr);
        sockfd_accept = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
        if (sockfd_accept == -1) {
            perror("[Error] Failed accept a connection");
            return 1;
        }

        info = malloc(sizeof(struct thread_info));
        info->sockfd = sockfd_accept;
        if (pthread_create(&info->tid, NULL, thread, info) != 0) { // Create a thread.
            perror("[Error] Failed to create a thread");
            close(sockfd_accept);
            close(sockfd);
            return 1;
        }
        printf("[Thread] (TID=%ld) Client is connected. (%s:%d)\n", info->tid, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        pthread_detach(info->tid);
    }

    return 0;
}
