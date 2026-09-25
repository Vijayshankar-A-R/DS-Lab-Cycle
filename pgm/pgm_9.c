#include <stdio.h>
#include <string.h>
#define LL_IMPLEMENTATION
#include "linklist.h"

typedef struct {
    double c;
    int e;
} term_t;

void init_poly(linklist_t *l) { ll_init(l, sizeof(term_t), NULL); }

void read_poly(linklist_t *l) {
    while (1) {
        int c;
        printf("Want to continue?(1): ");
        scanf("%d", &c);
        getc(stdin);
        if (c != 1)
            return;
        term_t t;
        scanf("%lf", &t.c);
        scanf("%d", &t.e);
        // printf("%lf %d\n", t.c, t.e);
        getc(stdin);
        ll_inserttail(l, &t);
    }
}

void mul_poly(linklist_t *c, linklist_t *a, linklist_t *b) {
    double coeff;
    int exp;
    for (ll_node *i = a->head; i; i = i->next) {
        for (ll_node *j = b->head; j; j = j->next) {
            term_t r = *(term_t *)i->data;
            term_t s = *(term_t *)j->data;
            coeff = r.c * s.c;
            exp = r.e + s.e;

            int found = 0;
            int y = 0;

            for (ll_node *k = c->head; k; k = k->next, y++) {
                term_t *t = (term_t *)k->data;
                if (t->e == exp) {
                    t->c += coeff;
                    found = 1;
                    break;
                } else if (t->e < exp)
                    break;
            }
            if (!found) {
                term_t x = (term_t){.c = coeff, .e = exp};
                ll_insert(c, y, &x);
            }
        }
    }
}

void print_poly(linklist_t *l) {
    for (ll_node *cur = l->head; cur; cur = cur->next) {
        term_t t = *(term_t *)cur->data;
        printf("%+gx%d ", t.c, t.e);
    }
    printf("\n");
}

int main() {

    linklist_t a, b, c;
    init_poly(&a);
    init_poly(&b);
    init_poly(&c);
    read_poly(&a);
    read_poly(&b);
    print_poly(&a);
    print_poly(&b);

    mul_poly(&c, &a, &b);
    print_poly(&c);

    ll_free(&a);
    ll_free(&b);
    ll_free(&c);

    return 0;
}
