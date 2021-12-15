#include <stdio.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

int main() {
    if (access("not_found.txt", F_OK) == -1) {
        printf("errno = %d\n", errno);
        perror("error message");
    }
    return 0;
}
