#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 10

int thread_cnt;
pthread_t tid[MAX_THREADS];

void* counting(void* arg) {
    int i, j, *ret;
    int index = *((int*)arg);

    sleep(rand() % 3);

    for (i = 1; i <= 5; i++) {
        for (j = 0; j < thread_cnt; j++) {
            if (j == index) {
                if (i == 5) {
                    printf("|FINISH");
                } else {
                    printf("| %d... ", i);
                }
            } else {
                printf("|      ");
            }
        }
        printf("|\n");
        sleep(1);
    }

    ret = (int*)malloc(sizeof(int));
    *ret = index;
    pthread_exit(ret);
}

int main(int argc, char* argv[]) {
    int i, j, *status, args[MAX_THREADS];

    if (argc != 2) {
        printf("Usage: ./counting <num of thread>\n");
        return 0;
    }

    thread_cnt = atoi(argv[1]);
    if (thread_cnt < 1 || thread_cnt > MAX_THREADS) {
        printf("Only values between 1 and %d are allowed for the number of threads.\n", MAX_THREADS);
        return 1;
    }

    printf("Thread Count: %d\n\n", thread_cnt);
    for (i = 0; i < thread_cnt; i++) {
        printf("|<TH %d>", i);
    }
    printf("|\n");

    srand(time(NULL));
    for (i = 0; i < thread_cnt; i++) {
        args[i] = i;
        if (pthread_create(&tid[i], NULL, counting, &args[i]) != 0) {
            perror("Failed to create a thread");
            break;
        }
    }

    for (j = 0; j < i; j++) {
        pthread_join(tid[j], (void**)&status);
        printf("Thread #%d returned %d\n", j, *status);
    }

    return 0;
}
