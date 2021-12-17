#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Before exec function.\n"); // printed

    if (execlp("ls", "ls", "-a", (char*)NULL) == -1) {
        perror("execlp");
        exit(1);
    }

    printf("After exec function.\n"); // not printed

    return 0;
}
