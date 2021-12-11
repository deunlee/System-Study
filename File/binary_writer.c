#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 100

char* names[] = { "Alice", "Bob", "Chris", "Dod", "Evan", 
                  "Fint", "Gregg", "Brendan", "Roy", "Susan"};
int name_cnt = sizeof(names) / sizeof(char*);

struct student {
    int   id;
    char  name[10];
    float score1;
    float score2;
};

int main(int argc, char* argv[]) {
    int i, n;
    struct student s;
    FILE* fp;
    
    srand(time(NULL));

    if (argc != 2) {
        printf("Usage: ./%s\b\b <output-file>\n", __FILE__);
        return 1;
    }

    fp = fopen(argv[1], "wb");
    if (fp == NULL) {
        perror("[Error] Failed to open a file");
        return 1;
    }

    n = (rand() % MAX_STUDENTS) + 1; // 학생수는 랜덤값 사용
    printf("Number of students: %d\n\n", n);
 
    for (i = 1; i <= n; i++) {
        s.id = 10000 + i;
        strcpy(s.name, names[rand() % name_cnt]);
        s.score1 = (rand() % 10000) / 100.0;
        s.score2 = (rand() % 10000) / 100.0;
        printf("%d %-10s : %5.2f / %5.2f\n", s.id, s.name, s.score1, s.score2);
    
        fwrite(&s, sizeof(s), 1, fp); // 구조체 기록
    }

    fclose(fp);
    return 0;
}