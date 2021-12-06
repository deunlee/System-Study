#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int parent() {
    printf("[Parent] I am your father.");
}

int child() {
    printf("[Child] I am child.");
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("[Parent] Failed to fork process");
        return 1;
    } else if (pid == 0) {
        printf("[Child] Forked successfully. (PID=%d, PPID=%d)\n", getpid(), getppid());
        return child();
    } else {
        printf("[Parent] Forked successfully. (PID=%d)\n", getpid());
        return parent();
    }
}
