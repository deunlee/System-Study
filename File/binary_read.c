#include <stdio.h>
#include <stdlib.h>

#define MAX_STUDENTS 100

struct student {
    int   id;
    char  name[10];
    float score1;
    float score2;
};

struct student_list { // linked-list
    struct student       data; // 데이터
    struct student_list* next; // 다음 노드 (자기 참조 구조체)
};

int main(int argc, char* argv[]) {
    int i, n;
    struct student_list* head, *tail, *temp;
    FILE* fp;
    
    if (argc != 2) {
        printf("Usage: ./%s\b\b <input-file>\n", __FILE__);
        return 1;
    }

    fp = fopen(argv[1], "rw");
    if (fp == NULL) {
        perror("[Error] Failed to open a file");
        return 1;
    }

    // 데이터 불러오기 (linked-list)
    // head는 고정 (순회 전용), tail에 노드를 추가하고 tail을 갱신한다.
    head = tail = malloc(sizeof(struct student_list)); // head와 tail은 항상 NULL이 아님
    while (1) {
        temp = malloc(sizeof(struct student_list)); // 새로 추가할 공간 먼저 생성
        if (!fread(&temp->data, sizeof(struct student), 1, fp)) {
            free(temp); // 추가할 내용이 없으면 메모리 반환
            break;
        }
        tail->next = temp;
        temp->next = NULL;
        tail = temp;
    }
    fclose(fp); // 파일 닫기
    
    printf("Enter index: ");
    scanf("%d", &n);
    if (n <= 0 || n > MAX_STUDENTS) {
        printf("[Error] Invalid index range.\n");
        return 1;
    }

    // 순회
    temp = head; // 첫 노드부터 시작
    for (i = 0; i < n; i++) { // n개의 노드 패스
        temp = temp->next;
        if (temp == NULL) {
            printf("[Error] Index not found.\n");
            return 1;
        }
    }
    printf("%d %-10s : %5.2f / %5.2f\n", temp->data.id, temp->data.name, temp->data.score1, temp->data.score2);
    
    // 메모리 반환
    temp = head;
    while (temp) {
        tail = temp->next;
        free(temp);
        temp = tail;
    }

    return 0;
}
