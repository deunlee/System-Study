#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t pid;
    FILE *fp;

    fp = fopen("test.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(fp, "Before fork\n");

    if (argc == 2 && strcmp("--fflush", argv[1]) == 0) {
        fflush(fp);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        fprintf(fp, "I'm a child.\n");
        fclose(fp);
        exit(0);
    } else {
        wait(NULL); // wait for a child.
        fprintf(fp, "I'm a parent.\n");
        fclose(fp);
    }

    return 0;
}
