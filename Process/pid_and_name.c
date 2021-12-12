#include <stdio.h>
#include <unistd.h>

int get_process_name_by_pid(int pid, char* name, size_t name_size) {
    char path[32];
    FILE* fp;
    size_t size;
    sprintf(path, "/proc/%d/cmdline", pid);
    fp = fopen(path, "r");
    if (fp == NULL) {
        name[0] = '\0';
        return 1;
    }
    size = fread(name, sizeof(char), name_size, fp);
    if (!size) {
        name[0] = '\0';
        return 2; 
    }
    name[size - 1] = '\0';
    fclose(fp);
    return 0;
}

int main() {
    char name[1024];
    
    get_process_name_by_pid(getpid(), name, sizeof(name));
    printf("PID=%d (%s)\n", getpid(), name);

    get_process_name_by_pid(getppid(), name, sizeof(name));
    printf("PPID=%d (%s)\n", getppid(), name);

    return 0;
}
