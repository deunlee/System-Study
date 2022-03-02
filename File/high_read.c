#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE *fp;
    char id[10];
    int grade1, grade2, grade3;
    float avg;

    if (argc != 2) {
        printf("< Usage: ./high4 filename >\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Open");
        exit(1);
    }

    fscanf(fp, "%s %d %d %d %f\n", id, &grade1, &grade2, &grade3, &avg);
    fprintf(stdout, "%s %d %d %d %.3f\n", id, grade1, grade2, grade3, avg);

    fscanf(fp, "%s %d %d %d %f\n", id, &grade1, &grade2, &grade3, &avg);
    fprintf(stdout, "%s %d %d %d %.3f\n", id, grade1, grade2, grade3, avg);
    
    fscanf(fp, "%s %d %d %d %f\n", id, &grade1, &grade2, &grade3, &avg);
    fprintf(stdout, "%s %d %d %d %.3f\n", id, grade1, grade2, grade3, avg);
    
    fclose(fp);

    return 0;
}
