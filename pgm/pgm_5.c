#include <stdio.h>
#include <string.h>
#define QUEUE_IMPLEMENTATION
#include "queue.h"

#define MAX_LEN         64
#define SECS_PER_PAGE   2   // 30 ppm

struct print_job {
    char    teacher[MAX_LEN];
    char    filename[MAX_LEN];
    int     pages;
};

typedef struct print_job job_t;

void print_job(job_t j) {
    printf("%s's %s(%d)\n", j.teacher, j.filename, j.pages);
}

int main() {
    queue_t pq; // printer queue
    q_init(&pq, sizeof(job_t), NULL);
    printf( "1. Add job\n"
            "2. View current job\n"
            "3. Print current job\n"
            "4. Print last wait time\n"
            "5. Print specific wait time\n"
            "6. Exit\n\n");
    char tmp[MAX_LEN];
    job_t j;
    int ch, s, r;
    dll_node *cur;
    r = 1;
    do {
        printf("Choice: ");
        scanf("%d", &ch);
        getc(stdin);
        switch (ch) {
            case 1:
                printf("Enter teacher's name: ");
                fgets(j.teacher, MAX_LEN, stdin);
                j.teacher[strcspn(j.teacher, "\n")] = '\0';

                printf("Enter file name: ");
                fgets(j.filename, MAX_LEN, stdin);
                j.filename[strcspn(j.filename, "\n")] = '\0';
                
                printf("Enter #pages: ");
                scanf("%d", &j.pages);
                getc(stdin);
                
                enqueue(&pq, &j);
                printf("queued: ");
                print_job(j);
                memset(&j, 0, sizeof(job_t));
                break;
            case 2:
                if (q_isempty(&pq)) printf("Empty queue\n");
                else {
                    peek(&pq, &j);
                    print_job(j);
                }
                break;
            case 3: 
                if (q_isempty(&pq)) printf("Empty queue\n");
                else {
                    dequeue(&pq, &j);
                    print_job(j);
                }
                break;
            case 4:
                s = 0;
                cur = pq.head;
                for (; cur && cur->next; cur = cur->next) s += ((job_t *)(cur->data))->pages * SECS_PER_PAGE;
                print_job(*(job_t *)cur->data);
                printf("%d secs left\n", s);
                break;
            case 5:
                printf("Enter filename you want to check: ");
                fgets(tmp, MAX_LEN, stdin);
                tmp[strcspn(tmp, "\n")] = '\0';
                cur = pq.head;
                s = 0;
                for (; cur && strcmp(tmp, ((job_t *)(cur->data))->filename); cur = cur->next) 
                    s += ((job_t *)(cur->data))->pages * SECS_PER_PAGE;
                print_job(*(job_t *)cur->data);
                printf("%d secs left\n", s);
                break;
            case 6:
                r = 0;
                break;
        }
    } while (r);
    q_free(&pq);
    return 0;
}
