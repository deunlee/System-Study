#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int  global_var;  // DATA
int* dynamic_var; // HEAP

int main() {
    pid_t pid;
    int local_var; // STACK
    
    dynamic_var  = (int*)malloc(sizeof(int));
    global_var   = 10;
    local_var    = 20;
    *dynamic_var = 30;
    
    printf("INIT: Default memory value. (global=DATA, local=STACK, dynamic=HEAP)\n");
    printf("[Parent] global=%d (0x%p), local=%d (0x%p), dynamic=%d (0x%p)\n",
            global_var, &global_var, local_var, &local_var, *dynamic_var, dynamic_var);

    printf("\n");
    printf("FIRST: After fork, child process modifies memory.\n");
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        global_var++;
        local_var++;
        (*dynamic_var)++;
        printf("[Child]  global=%d (0x%p), local=%d (0x%p), dynamic=%d (0x%p)\n",
                global_var, &global_var, local_var, &local_var, *dynamic_var, dynamic_var);
        exit(0);
    } else {
        sleep(2);
        printf("[Parent] global=%d (0x%p), local=%d (0x%p), dynamic=%d (0x%p)\n",
                global_var, &global_var, local_var, &local_var, *dynamic_var, dynamic_var);
    }

    printf("\n");
    printf("SECOND: After fork, parent process modifies memory.\n");
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        sleep(2);
        printf("[Child]  global=%d (0x%p), local=%d (0x%p), dynamic=%d (0x%p)\n",
                global_var, &global_var, local_var, &local_var, *dynamic_var, dynamic_var);
        exit(0);
    } else {
        global_var++;
        local_var++;
        (*dynamic_var)++;
        printf("[Parent] global=%d (0x%p), local=%d (0x%p), dynamic=%d (0x%p)\n",
                global_var, &global_var, local_var, &local_var, *dynamic_var, dynamic_var);
        sleep(4);
    }

    return 0;
}
