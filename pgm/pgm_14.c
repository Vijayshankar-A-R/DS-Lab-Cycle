#include <stdio.h>
#include <string.h>
#define HEAP_IMPLEMENTATION
#include "heap.h"

#define LEN 64

enum Priority { NORMAL, DEFENCE, SENIOR, DIFFERENTLY_ABLED };

const char *ptos[4] = {"NORMAL", "DEFENCE", "SENIOR", "DIFFERENTLY ABLED"};

struct post_office_token {
    int token_id;
    enum Priority p;
    char name[LEN];
};

typedef struct post_office_token token_t;

int token_cmp(const void *a, const void *b) {
    const token_t *t1, *t2;
    t1 = (const token_t *)a;
    t2 = (const token_t *)b;
    int d = t1->p - t2->p;
    return d ? d : t2->token_id - t1->token_id;
}

void print_token(token_t t) {
    printf("%d. %s: %s\n", t.token_id, t.name, ptos[t.p]);
}

int main() {
    hp_t pq;
    hp_init(&pq, sizeof(token_t), token_cmp, NULL);

    printf("Normal: 0\n"
           "Defense: 1\n"
           "Senior: 2\n"
           "Differently abled: 3\n\n");

    printf("1. Take token\n"
           "2. Serve customer\n"
           "3. Exit\n\n");
    int c, r, t, s;
    token_t tk;
    t = 1;
    r = 1;

    do {
        scanf("%d", &c);
        getc(stdin);
        switch (c) {
        case 1:
            printf("Enter name: ");
            fgets(tk.name, LEN, stdin);
            tk.name[strcspn(tk.name, "\n")] = '\0';

            printf("Enter priority: ");
            scanf("%d", (int *)&tk.p);
            getc(stdin);

            tk.token_id = t++;
            hp_insert(&pq, &tk);
            printf("Inserted: ");
            print_token(tk);
            break;
        case 2:
            s = hp_popmax(&pq, &tk);
            printf("Served: ");
            if (s)
                print_token(tk);
            else
                printf("Empty\n");
            break;
        case 3:
            r = 0;
            break;
        default:
            printf("Invalid choice\n");
            break;
        }

    } while (r);

    hp_free(&pq);
    return 0;
}
