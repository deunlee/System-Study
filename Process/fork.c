#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int parent() {
    printf("[Parent] I am your father.\n");
    return 0;
}

int child() {
    printf("[Child] I am a child.\n");
    return 123; // Return value is 8 bits.
}

int main() {
    pid_t pid = fork();
    int ret, wstatus;

    if (pid < 0) {
        perror("[Parent] Failed to fork process");
        return 1;
    } else if (pid == 0) {
        printf("[Child] Forked successfully. (PID=%d, PPID=%d)\n", getpid(), getppid());
        ret = child();
    } else {
        printf("[Parent] Forked successfully. (PID=%d)\n", getpid());
        ret = parent();
        wait(&wstatus);
        // https://man7.org/linux/man-pages/man2/wait.2.html
        printf("[Parent] Child returned %d.\n", WEXITSTATUS(wstatus));
    }

    return ret;
}
