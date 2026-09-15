#include <stdio.h>
#include <stdlib.h> // calloc, free
#include <string.h> // memset
#include <assert.h>
struct tuple {
    int i;  // row
    int j;  // column
    int v;  // value
};

struct sparse_matrix {
    int n;      // Number of non-zero tuples
    int r;      // Number of rows
    int c;      // Number of cols
    struct tuple *nonzeroes;
};

typedef struct sparse_matrix spmat_t;

void read_mat(spmat_t *sp) {
    printf("#Rows: ");
    scanf("%d", &sp->r);
    printf("#Column: ");
    scanf("%d", &sp->c);
    int n;
    printf("#Nonzeroes: ");
    scanf("%d", &n);
    sp->n = n;
    sp->nonzeroes = (struct tuple *)calloc(n, sizeof(struct tuple));
    while (n--) {
        printf(">>");
        struct tuple *c = &sp->nonzeroes[n];
        scanf("%d@%d,%d", &c->v, &c->i, &c->j);
    }
}

void destroy_mat(spmat_t *sp) {
    if (sp->nonzeroes) free(sp->nonzeroes);
    memset(sp, 0, sizeof(spmat_t));
}

int get_elem(spmat_t sp, int i, int j) {
    for (int k = 0; k < sp.n; ++k)
        if (sp.nonzeroes[k].i == i && sp.nonzeroes[k].j == j)
            return sp.nonzeroes[k].v;
    return 0;
}

void print_mat(spmat_t sp) {
    for (int i = 0; i < sp.r; ++i) {
        for (int j = 0; j < sp.c; ++j)
            printf("%d ", get_elem(sp, i, j));
        printf("\n");
    }
}

void mat_add(spmat_t a, spmat_t b, spmat_t *c) {
    assert(a.r == b.r && a.c == b.c);
    c->r = a.r;
    c->c = a.c;
    int cnt = 0;
    int v;
    c->nonzeroes = (struct tuple *)calloc(a.n + b.n, sizeof(struct tuple));
    for (int i = 0; i < a.r; ++i)
        for (int j = 0; j < a.c; ++j)
            if ( (v = get_elem(a, i, j) + get_elem(b, i, j)) != 0)
                c->nonzeroes[cnt++] = (struct tuple) {
                        .i = i,
                        .j = j,
                        .v = v
                };
    c->n = cnt;
}

void mat_trans(spmat_t a, spmat_t *t) {
    t->r = a.c;
    t->c = a.r;
    t->n = a.n;
    t->nonzeroes = (struct tuple*)calloc(a.n, sizeof(struct tuple));
    for (int i = 0; i < a.n; ++i) {
        struct tuple n = a.nonzeroes[i];
        t->nonzeroes[i] = (struct tuple) {
                .i = n.j,
                .j = n.i,
                .v = n.v
        };
    }
}

int main() {
    spmat_t a, b, c, t, d;
    printf("Static Building: \n");
    read_mat(&a);
    print_mat(a);

    printf("Moving Vehicle: \n");
    read_mat(&b);
    print_mat(b);

    printf("Animated Characters: \n");
    read_mat(&c);
    print_mat(c);

    printf("Transformation Matrix: \n");
    mat_add(a, b, &t);
    mat_add(t, c, &d);
    destroy_mat(&t); // free up t before reuse
    mat_trans(d, &t);
    print_mat(t);

    destroy_mat(&a);
    destroy_mat(&b);
    destroy_mat(&c);
    destroy_mat(&t);
    destroy_mat(&d);
    return 0;
}
