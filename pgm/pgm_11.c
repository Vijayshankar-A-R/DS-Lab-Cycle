#include <assert.h>
#include <stdio.h>
#include <string.h>
#define LL_IMPLEMENTATION
#include "linklist.h"

#define AUTO_COMPACT    // remove to disable auto-compaction on free

typedef struct {
    char name[10];
    int start;
    int size;
    int alloc;
} chunk_t;

void __alloc(linklist_t *mem, char name[10], int start, int size) {
    int i = 0;
    for (ll_node *cur = mem->head; cur; cur = cur->next, i++) {
        chunk_t *ch = (chunk_t *)cur->data;
        int st, sz;
        st = ch->start;
        sz = ch->size;
        if (start < st)
            continue;
        if (start >= st + sz)
            continue;
        assert(!ch->alloc);
        chunk_t prev;
        prev.start = st;
        prev.size = start - st;
        prev.alloc = 0;
        chunk_t new;
        memcpy(new.name, name, 10);
        new.start = start;
        new.size = size;
        new.alloc = 1;
        chunk_t left;
        left.start = start + size;
        left.size = sz - prev.size - new.size;
        left.alloc = 0;
        chunk_t tmp;
        ll_delete(mem, i, &tmp);
        if (prev.size > 0)
            ll_insert(mem, i, &prev);
        else
            i--;
        ll_insert(mem, i + 1, &new);
        if (left.size > 0)
            ll_insert(mem, i + 2, &left);
        break;
    }
#ifdef DEBUG
    printf("Allocated %s: %dKB\n", name, size);
#endif
}

void __free(linklist_t *mem, char name[10]) {
    int i = 0;
    ll_node *cur = mem->head;
    for (; cur; cur = cur->next, ++i) {
        chunk_t *ch = (chunk_t *)cur->data;
        if (ch->alloc && !strcmp(ch->name, name)) {
            ch->alloc = 0;
#ifdef AUTO_COMPACT
            chunk_t prev;
            while (ll_getelem(mem, i - 1, &prev) && !prev.alloc) {
                ch->start = prev.start;
                ch->size += prev.size;
                ll_delete(mem, i - 1, &prev);
                if (!i--)
                    break;
            }
            ll_node *next = cur->next;
            chunk_t nxtch = *(chunk_t *)next->data;
            while (!nxtch.alloc) {
                ch->size += nxtch.size;
                next = next->next;
                if (!next) {
                    ll_delete(mem, i + 1, &prev);
                    break;
                }
                nxtch = *(chunk_t *)next->data;
                ll_delete(mem, i + 1, &prev);
            }
#endif
#ifdef DEBUG
            printf("Freed: %s\n", name);
#endif
            return;
        }
    }
}

void ff(linklist_t *mem, char name[10], int size) {
    for (ll_node *cur = mem->head; cur; cur = cur->next) {
        chunk_t *ch = (chunk_t *)cur->data;
        if (!ch->alloc && ch->size >= size)
            return __alloc(mem, name, ch->start, size);
    }
    printf("No space!\n");
}

void bf(linklist_t *mem, char name[10], int size) {
    chunk_t *cand = NULL;
    for (ll_node *cur = mem->head; cur; cur = cur->next) {
        chunk_t *ch = (chunk_t *)cur->data;
        if (!ch->alloc && ch->size >= size) {
            if (cand)
                cand = cand->size > ch->size ? ch : cand;
            else
                cand = ch;
        }
    }
    if (cand)
        return __alloc(mem, name, cand->start, size);
    else
        printf("No space!\n");
}

void wf(linklist_t *mem, char name[10], int size) {
    chunk_t *cand = NULL;
    for (ll_node *cur = mem->head; cur; cur = cur->next) {
        chunk_t *ch = (chunk_t *)cur->data;
        if (!ch->alloc && ch->size >= size) {
            if (cand)
                cand = cand->size < ch->size ? ch : cand;
            else
                cand = ch;
        }
    }
    if (cand)
        return __alloc(mem, name, cand->start, size);
    else
        printf("No space!\n");
}

void __print(linklist_t *mem) {
    for (ll_node *cur = mem->head; cur; cur = cur->next) {
        chunk_t ch = *(chunk_t *)cur->data;
        // printf("print\n");
        printf("%s: start: %d, end: %d\n", ch.alloc ? ch.name : "Free",
               ch.start, ch.start + ch.size);
    }
    printf("==================================\n");
}

int main() {
    linklist_t mem;
    ll_init(&mem, sizeof(chunk_t), NULL);
    chunk_t init = (chunk_t){.name = {0}, .start = 0, .size = 4000, .alloc = 0};
    ll_inserthead(&mem, &init);

#ifdef AUTO_COMPACT
    printf("Please note: In this implementation"
           "memory frees initiates auto-compaction.\n\n");
#endif
    int r = 1;
    int c, s;
    char name[10];
    printf( "1. First fit\n"
            "2. Best fit\n"
            "3. Worst fit\n"
            "4. Free\n"
            "5. Print\n"
            "6. Exit\n\n"
          );
    do {
        printf("Choice: ");
        scanf("%d", &c);
        getc(stdin);
        switch (c) {
        case 1:
            printf("Enter program name: ");
            fgets(name, 10, stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Enter size: ");
            scanf("%d", &s);
            getc(stdin);
            ff(&mem, name, s);
            break;
        case 2:
            printf("Enter program name: ");
            fgets(name, 10, stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Enter size: ");
            scanf("%d", &s);
            getc(stdin);
            bf(&mem, name, s);
            break;
        case 3:
            printf("Enter program name: ");
            fgets(name, 10, stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Enter size: ");
            scanf("%d", &s);
            getc(stdin);
            wf(&mem, name, s);
            break;
        case 4:
            printf("Enter program name: ");
            fgets(name, 10, stdin);
            name[strcspn(name, "\n")] = '\0';
            __free(&mem, name);
            break;
        case 5:
            __print(&mem);
            break;
        case 6:
            r = 0;
            break;
        default:
            printf("Invalid option\n");
            break;
        }
    } while (r);

    ll_free(&mem);
    return 0;
}
