#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE *fp;

    if (argc != 2) {
        printf("< Usage: ./high3 filename >\n");
        return 1;
    }

    fp = fopen(argv[1], "w");
    if (fp == NULL) {
        perror("Open");
        exit(1);
    }

    fprintf(fp, "%s %d %d %d %2.2f\n", "2015123", 80, 90, 100, 90.00);
    fprintf(fp, "%s %d %d %d %2.2f\n", "2016123", 50, 80, 60, 66.22);
    fprintf(fp, "%s %d %d %d %2.2f\n", "2017123", 70, 20, 70, 55.55);

    fclose(fp);

    return 0;
}
