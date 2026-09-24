#include <assert.h>
#include <stdio.h>
#include <string.h>
#define LL_IMPLEMENTATION
#include "linklist.h"

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

void test_alloc_strat(linklist_t *mem,
                      void (*alloc_fn)(linklist_t *mem, char name[10],
                                       int size)) {

    ll_init(mem, sizeof(chunk_t), NULL);
    chunk_t init = (chunk_t){.name = {0}, .start = 0, .size = 4000, .alloc = 0};
    ll_inserthead(mem, &init);

    //	__print(mem);

    char name[10];

    strcpy(name, "P5");
    __alloc(mem, name, 0, 10);
    strcpy(name, "P6");
    __alloc(mem, name, 310, 90);
    strcpy(name, "P1");
    __alloc(mem, name, 1000, 500);
    strcpy(name, "P2");
    __alloc(mem, name, 1850, 150);
    strcpy(name, "P4");
    __alloc(mem, name, 2200, 100);
    strcpy(name, "P3");
    __alloc(mem, name, 3150, 350);

    printf("\n\nStarting mem: ");
    __print(mem);

    strcpy(name, "P7");
    alloc_fn(mem, name, 115);

    strcpy(name, "P10");
    alloc_fn(mem, name, 650);

    strcpy(name, "P3");
    __free(mem, name);

    strcpy(name, "P1");
    __free(mem, name);

    strcpy(name, "P6");
    __free(mem, name);

    strcpy(name, "P8");
    alloc_fn(mem, name, 200);

    strcpy(name, "P5");
    __free(mem, name);

    strcpy(name, "P2");
    __free(mem, name);

    strcpy(name, "P9");
    alloc_fn(mem, name, 37);

    strcpy(name, "P10");
    __free(mem, name);

    strcpy(name, "P9");
    __free(mem, name);

    strcpy(name, "P4");
    __free(mem, name);

    printf("Final Memory Layout:\n");
    __print(mem);

    ll_free(mem);
}

int main() {
    linklist_t mem;

    printf("Please note: In this implementation"
           "memory frees auto-compact.\n\n");

    printf("\nFirst fit: ");
    test_alloc_strat(&mem, ff);
    printf("\nBest fit: ");
    test_alloc_strat(&mem, bf);
    printf("\nWorst fit: ");
    test_alloc_strat(&mem, wf);

    return 0;
}
