#include <stdio.h>
#include <string.h>
#define QUEUE_IMPLEMENTATION
#include "queue.h"

#define MAX_LEN 32

typedef struct token {
    int     token_no;
    int     tickets;
    char    name[MAX_LEN];
} token_t;

void print_token(token_t t) {
    printf("%d. %s(%d)\n", t.token_no, t.name, t.tickets);
}

int main() {
    queue_t q;
    q_init(&q, sizeof(token_t), NULL);

    printf( "1. Request token\n"
            "2. Fill form\n"
            "3. Current cust.\n"
            "4. No. of waiting cust.\n"
            "5. List cust.\n"
            "6. Exit\n\n"
          );
    int ch, r, n;
    token_t t = {0};
    n = 1;  // token no
    r = 1;
    do {
        printf("Choice: ");
        scanf("%d", &ch);
        getc(stdin);
        switch (ch) {
            case 1:
                t.token_no = n++;

                printf("Enter name: ");
                fgets(t.name, MAX_LEN, stdin);
                t.name[strcspn(t.name, "\n")] = '\0';

                printf("Enter #tickets: ");
                scanf("%d", &t.tickets);

                enqueue(&q, &t);
                printf("Queued: ");
                print_token(t);
                memset(&t, 0, sizeof(token_t));
                break;
            case 2:
                if (q_isempty(&q)) printf("Empty queue\n");
                else {
                    dequeue(&q, &t);
                    print_token(t);
                    if (--t.tickets) enqueue(&q, &t);
                }
                break;
            case 3:
                if (q_isempty(&q)) printf("Empty queue\n");
                else {
                    peek(&q, &t);
                    print_token(t);
                }
                break;
            case 4:
                if (q_isempty(&q)) printf("Empty queue\n");
                else printf("Waiting: %zu\n", q_size(&q));
                break;
            case 5:
                for (dll_node *cur = q.head; cur; cur = cur->next)
                    print_token(*(token_t *)(cur->data));
                break;
            case 6:
                r = 0;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    } while (r);

    q_free(&q);
    return 0;
}
