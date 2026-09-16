#include <stdio.h>
#include <string.h>
#define STACK_IMPLEMENTATION
#include "stack.h"

#define LEN 32

typedef char url_t[LEN];

void __ll_rev_print(ll_node *n) {
    if (!n) return;
    __ll_rev_print(n->next);
    printf("%s", *(url_t*)(n->data));
}

void print_url(stack_t s) {
    __ll_rev_print(s.head);
    printf("\n");
}

int main() {
    stack_t undo, redo;     // undo stack holds the actual URL
    stack_init(&undo, sizeof(url_t), NULL);
    stack_init(&redo, sizeof(url_t), NULL);
    int ch, r;
    url_t tmp;

    r = 1;
    printf( "1. Goto\n"
            "2. Back\n"
            "3. Forward\n"
            "4. Print\n"
            "5. Exit\n\n"
          );
    do {
        printf("Choice: ");
        scanf("%d", &ch);
        getc(stdin);
        switch (ch) {
            case 1:
                printf("Enter next path (eg: home/): ");
                fgets(tmp, LEN, stdin);
                tmp[strcspn(tmp, "\n")] = '\0';
                stack_push(&undo, tmp);

                stack_free(&redo);
                stack_init(&redo, sizeof(url_t), NULL); // reinit redo
                break;
            case 2:
                if (stack_isempty(&undo)) printf("Empty URL\n");
                else {
                    stack_pop(&undo, tmp);
                    stack_push(&redo, tmp);
                }
                break;
            case 3:
                if (stack_isempty(&redo)) printf("Empty history\n");
                else {
                    stack_pop(&redo, tmp);
                    stack_push(&undo, tmp);
                }
                break;
            case 4:
                print_url(undo);
                break;
            case 5:
                r = 0;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    } while (r);

    stack_free(&undo);
    stack_free(&redo);
    return 0;
}
