#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char* argv[2];
    char* envp[2];

    argv[0] = "env";
    argv[1] = NULL;

    envp[0] = "MY_NAME=DEUN_LEE";
    envp[1] = NULL;

    printf("Before exec function.\n"); // printed

    if (execve("/usr/bin/env", argv, envp) == -1) {
        perror("execve");
        exit(1);
    }

    printf("After exec function.\n"); // not printed

    return 0;
}
