#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char* argv[3];
    
    argv[0] = "ls";
    argv[1] = "-a";
    argv[2] = NULL;

    printf("Before exec function.\n"); // printed

    if (execv("/usr/bin/ls", argv) == -1) {
        perror("execv");
        exit(1);
    }

    printf("After exec function.\n"); // not printed

    return 0;
}
