#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libgen.h>

#define BUFFER_SIZE   4096
#define BAR_LENGTH      60

struct progress_bar {
    size_t total;
    size_t current;
    size_t delta;
    struct timeval old_time;
};
struct progress_bar bar;

time_t diff_time(struct timeval* a, struct timeval* b) {
    return (b->tv_sec - a->tv_sec) * 1000000 + (b->tv_usec - a->tv_usec);
}

void print_size_with_unit(double size) {
    if (size < 1024.0) { printf("%4.0lf B", size); return; }
    size /= 1024.0;
    if (size < 1024.0) { printf("%4.1f KB", size); return; }
    size /= 1024.0;
    if (size < 1024.0) { printf("%4.1f MB", size); return; }
    size /= 1024.0;
    printf("%4.1f GB", size);
}

void progress_bar_init(size_t total) {
    memset(&bar, 0, sizeof(bar));
    bar.total = total;
    printf("\n");
}

void progress_bar_add(size_t delta) {
    int i, j;
    struct timeval cur_time;
    time_t diff;
    bar.current += delta;
    bar.delta   += delta;
    gettimeofday(&cur_time, NULL);
    diff = diff_time(&bar.old_time, &cur_time);
    if (diff >= 500 * 1000 || bar.current >= bar.total) { // Update every 500ms.
        printf("\r%3d%% [", (int)((double)bar.current / bar.total * 100.0));
        j = (double)bar.current / bar.total * BAR_LENGTH;
        for (i = 0; i < j - 1; i++) printf("="); printf(">");
        for (i = 0; i < BAR_LENGTH - j; i++) printf(" "); printf("] ");
        print_size_with_unit((double)bar.delta * 1000.0 / diff * 1000.0);
        printf("/s          ");
        fflush(stdout);
        bar.delta    = 0;
        bar.old_time = cur_time;
    }
}

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    size_t size, total_size;
    FILE* fp;
    char* buffer;

    if (argc != 4) { // Print usage.
        printf("Usage: ./client <ip> <port> <file>\n");
        return 0;
    }
    if (access(basename(argv[3]), F_OK) == 0) {
        printf("[Error] The file already exists.\n");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Error] Cannot connect to the server");
        return 1;
    }
    printf("[Client] Connected to the server. (%s:%s)\n", argv[1], argv[2]);

    // STEP 1. Check the file name and send it.
    size = strlen(argv[3]) + 1;
    if (size > BUFFER_SIZE) {
        perror("[Error] File name is too long");
        close(sockfd);
        return 1;
    }
    if (send(sockfd, &size, sizeof(size), 0) == -1) {
        perror("[Error] Failed to send length of file name");
        close(sockfd);
        return 1;
    }
    if (send(sockfd, argv[3], size, 0) == -1) {
        perror("[Error] Failed to send file name");
        close(sockfd);
        return 1;
    }

    // STEP 2. Create the file.
    fp = fopen(basename(argv[3]), "wb");
    if (fp == NULL) {
        perror("[Error] Failed to create the file");
        close(sockfd);
        return 1;
    }

    // STEP 3. Receive the file size for the progress bar.
    if (recv(sockfd, &total_size, sizeof(total_size), MSG_WAITALL) != sizeof(total_size)) {
        perror("[Error] Failed to receive file size");
        close(sockfd);
        return 1;
    }
    printf("[Client] File size is %ld bytes. (", total_size);
    print_size_with_unit(total_size);
    printf(")\n");
    progress_bar_init(total_size);

    // STEP 4. Receive the file and save.
    buffer = malloc(BUFFER_SIZE); // Allocate buffer memory.
    while ((size = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        progress_bar_add(size);
        fwrite(buffer, size, 1, fp);
    }
    // If recv() returns -1, or if all bytes are not received, treat it as a socket error.
    if (size == -1 || bar.current != bar.total) {
        printf("\n[Error] Failed to receive file data.\n");
        close(sockfd);
        fclose(fp);
        free(buffer);
        return 1;
    }
    printf("\n\n");
    printf("[Client] Received complete!\n");

    // STEP 5. Clean up.
    close(sockfd);
    fclose(fp);
    free(buffer);

    return 0;
}
