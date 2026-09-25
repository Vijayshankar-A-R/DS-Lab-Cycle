#include <alloca.h> // alloca
#include <stddef.h> // size_t
#include <stdio.h>
#define HEAP_IMPLEMENTATION
#include "heap.h"
#define QUICK_SORT_IMPL
#include "sort.h"

struct arr_view {
    int *arr; // array start
    size_t i; // current view
    size_t n; // capacity
};

int min_cmp(const void *a, const void *b) {
    const struct arr_view *c, *d;
    c = (const struct arr_view *)a;
    d = (const struct arr_view *)b;
    return d->arr[d->i] - c->arr[c->i];
}

int int_cmp(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

int main() {
    int k, c, i;
    struct arr_view av;
    hp_t heap;
    hp_init(&heap, sizeof(struct arr_view), min_cmp, NULL);
    printf("Enter k: ");
    scanf("%d", &k);
    int *arrs[k];
    for (i = 0; i < k; ++i) {
        int n;
        printf("Enter length of %dth arr: ", i + 1);
        scanf("%d", &n);
        arrs[i] = alloca(sizeof(int) * n);
        for (int j = 0; j < n; ++j)
            scanf("%d", &arrs[i][j]);
        quick_sort(arrs[i], n, sizeof(int), int_cmp);
        av.arr = arrs[i];
        av.i = 0;
        av.n = n;
        hp_insert(&heap, &av);
        c += n;
    }

    int res[c];
    i = 0;
    while (hp_popmax(&heap, &av)) {
        if (av.i == av.n)
            continue;
        res[i++] = av.arr[av.i++];
        hp_insert(&heap, &av);
    }

    for (i = 0; i < c; ++i)
        printf("%d ", res[i]);
    printf("\n");

    hp_free(&heap);
    return 0;
}
