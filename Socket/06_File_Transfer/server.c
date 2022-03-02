#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS          5 // backlog
#define BUFFER_SIZE       1024
#define DEFAULT_PORT     55555
#define DEFAULT_IP   "0.0.0.0"

int main(int argc, char* argv[]) {
    int sockfd, sockfd_accept, option = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    size_t size, total_size;
    FILE* fp;
    char* buffer = malloc(BUFFER_SIZE);
    int is_error = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot bind a socket");
        return 1;
    }

    listen(sockfd, MAX_CLIENTS);
    printf("[Server] Server is running at %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    client_addr_size = sizeof(client_addr);
    sockfd_accept = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
    if (sockfd_accept == -1) {
        perror("[Error] Failed accept a connection");
        return 1;
    }
    printf("[Server] Client is connected. (%s:%d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // STEP 1. Receive requested file name from the client.
    if (recv(sockfd_accept, &size, sizeof(size), MSG_WAITALL) == -1) {
        perror("[Error] Failed to receive length of file name");
        return 1;
    }
    if (recv(sockfd_accept, buffer, size, MSG_WAITALL) == -1) {
        perror("[Error] Failed to receive a file name");
        return 1;
    }
    printf("[Server] Requested file: %s\n", buffer);

    // STEP 2. Open the file.
    fp = fopen(buffer, "rb");
    if (fp == NULL) {
        perror("[Error] Failed to open the file");
        is_error = 1;
    }

    // STEP 3. Send the file size first for the client's progress bar printing.
    if (!is_error) {
        fseek(fp, 0, SEEK_END);
        total_size = ftell(fp); // Get file size.
        fseek(fp, 0, SEEK_SET);
        if (send(sockfd_accept, &total_size, sizeof(total_size), 0) == -1) {
            perror("[Error] Failed to send a file data");
            is_error = 1;
        }
    }

    // STEP 4. Read the file data and send to client.
    if (!is_error) {
        while (size = fread(buffer, 1, BUFFER_SIZE, fp)) {
            if (send(sockfd_accept, buffer, size, 0) == -1) {
                perror("[Error] Failed to send a file data");
                is_error = 1;
                break;
            }
        }
    }
    if (!is_error) {
        printf("[Server] File transfer complete!\n");
    }

    // STEP 5. Clean up.
    close(sockfd_accept);
    close(sockfd);
    fclose(fp);
    free(buffer);

    return is_error;
}
