#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS          5 // backlog
#define BUFFER_SIZE       1024
#define DEFAULT_PORT     55555
#define DEFAULT_IP   "0.0.0.0"

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
    printf("[Child] (PID=%d) Client says: %s\n", getpid(), buffer);

    close(sockfd);
    free(buffer);
    return 0;
}

void signal_handler(int signal) {
    pid_t pid;
    int status;
    psignal(signal, "[Server] Received signal");
    if (signal == SIGCHLD) { // When the child process exits
        // To wait for any child process, use -1 as PID. Use WNOHANG flag for non-blocking mode.
        // If several children are terminated at the same time, 
        // SIGCHLD signal may be delivered less than the number of children, so while loop should be used.
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            if (debug) {
                printf("[Server] (PID=%d) Child returned: %d\n", pid, WEXITSTATUS(status));
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int sockfd, sockfd_accept, option = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    pid_t pid;
    struct sigaction act;

    act.sa_flags   = 0;
    act.sa_handler = signal_handler;
    sigaction(SIGCHLD, &act, NULL); // Register signal for child process return handling.

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
            if (errno == EINTR) {
                continue; // Retry when accpet() is interrupted by receiving a signal from the child.
            }
            perror("[Error] Failed accept a connection");
            close(sockfd);
            return 1;
        }

        pid = fork(); // Create a child process.
        if (pid == 0) { // child code
            printf("[Child] (PID=%d) Forked successfully.\n", getpid());
            printf("[Child] (PID=%d) Client is connected. (%s:%d)\n", getpid(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            close(sockfd); // Since the listen socket is not used in child, close it.
            return communicate(sockfd_accept);
        } else if (pid < 0) {
            perror("[Error] Failed to create a child process");
            close(sockfd_accept);
            close(sockfd);
            return 1;
        }
        close(sockfd_accept);
    }

    return 0;
}
