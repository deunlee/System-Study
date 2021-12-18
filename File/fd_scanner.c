#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>

int is_listening_socket(int fd) {
    int flag;
    socklen_t len = sizeof(flag);

    if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &flag, &len) != -1) {
        if (flag) {
            return 1;
        }
    }
    return 0;
}

void print_fd_info(int fd) {
    struct stat fd_stat;

    if (fstat(fd, &fd_stat) == -1) {
        if (errno == EBADF) { // not opened
            return;
        } else {
            perror("fstat");
        }
    }

    // https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html
    if (S_ISDIR(fd_stat.st_mode)) {
        printf("File descriptor %d is a directory.\n", fd);
    } else if (S_ISCHR(fd_stat.st_mode)) {
        printf("File descriptor %d is a character special file.\n", fd);
    } else if (S_ISBLK(fd_stat.st_mode)) {
        printf("File descriptor %d is a block special file.\n", fd);
    } else if (S_ISREG(fd_stat.st_mode)) {
        printf("File descriptor %d is a regular file.\n", fd);
    } else if (S_ISFIFO(fd_stat.st_mode)) {
        printf("File descriptor %d is a FIFO special file, or a pipe.\n", fd);
    } else if (S_ISLNK(fd_stat.st_mode)) {
        printf("File descriptor %d is a symbolic link.\n", fd);
    } else if (S_ISSOCK(fd_stat.st_mode)) {
        if (is_listening_socket(fd)) {
            printf("File descriptor %d is a listening socket.\n", fd);
        } else {
            printf("File descriptor %d is a regular socket.\n", fd);
        }
    } else {
        printf("File descriptor %d is a unknown file.\n", fd);
    }
}

void scan_file_descriptors() {
    int i;
    struct rlimit rlim;

    if (getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
        perror("getrlimit");
        exit(1);
    }

    for (i = 0; i < rlim.rlim_cur; ++i) {
        print_fd_info(i);
    }
}

int create_listening_socket() {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // listening socket

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(55555);
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, 1) == -1) {
        perror("listen");
        exit(1);
    }

    return sockfd;
}

int create_regular_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int main() {
    // stdin=0, stdout=1, stderr=2
    FILE* fp           = fopen("test.txt", "w");    // fd=3
    int sockfd_listen  = create_listening_socket(); // fd=4
    int sockfd_regular = create_regular_socket();   // fd=5

    scan_file_descriptors(); // scan

    close(sockfd_listen);
    close(sockfd_regular);
    fclose(fp);

    return 0;
}
