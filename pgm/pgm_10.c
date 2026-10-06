#include <stdio.h>
#include <string.h>
#define LL_IMPLEMENTATION
#include "linklist.h"

#define LEN 64

typedef struct {
    char app_name[LEN];
    int frq;
} app_t;

int main() {

    linklist_t freq_used;
    ll_init(&freq_used, sizeof(app_t), NULL);

    int c, f, r, i;
    char tmp[LEN];
    app_t app;
    r = 1;

    printf("1. Open\n"
           "2. Clean\n"
           "3. Exit\n\n");

    do {
        scanf("%d", &c);
        getc(stdin);

        switch (c) {
        case 1:
            printf("Enter name of app to open: ");
            fgets(tmp, LEN, stdin);
            tmp[strcspn(tmp, "\n")] = '\0';
            f = 0;
            i = 0;
            {
                int del_idx, ist_idx;
                del_idx = ist_idx = -1;
                for (ll_node *cur = freq_used.head; cur; cur = cur->next, i++) {
                    app_t curapp = *(app_t *)cur->data;
                    if (!f && !strcmp(tmp, curapp.app_name)) {
                        //ll_delete(&freq_used, i, &app);
                        app = curapp;
                        app.frq++;
                        del_idx = i;
                        f = 1;
                        continue;
                    }
                    if (f) {
                        if (app.frq < curapp.frq) {
                            ist_idx = i;
                            break;
                        }
                    }
                }
                if (ist_idx != -1)
                    ll_insert(&freq_used, ist_idx, &app);
                else if (f)
                    ll_inserttail(&freq_used, &app);
                if (del_idx != -1)
                    ll_delete(&freq_used, del_idx, &app);
            }
            if (!f) {
                strcpy(app.app_name, tmp);
                app.frq = 1;
                ll_inserthead(&freq_used, &app);
            }
            memset(tmp, 0, LEN);
            break;
        case 2:
            i = ll_deletehead(&freq_used, &app);
            if (i)
                printf("Deleted app: %s\n", app.app_name);
            else
                printf("Empty\n");
            break;
        case 3:
            r = 0;
            break;
        default:
            printf("Invalid option\n");
            break;
        }
    } while (r);

    ll_free(&freq_used);

    return 0;
}
