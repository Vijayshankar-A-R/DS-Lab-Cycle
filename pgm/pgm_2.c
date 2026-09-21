#include <stdio.h>

// Nth degree truncated polynomial unit
struct ntru {
    int coeff[7]; // Max degree is 6
};

typedef struct ntru poly_t;

void poly_read(poly_t *p) {
    // Accept input as sequence of doubles from degree 6 to 0
    for (int i = 6; i >= 0; --i)
        scanf("%d", &p->coeff[i]);
    getc(stdin);
    // Convert to mod 3
    for (int i = 0; i < 7; ++i)
        p->coeff[i] = p->coeff[i] % 3;
}

poly_t poly_add(poly_t a, poly_t b) {
    poly_t c;
    for (int i = 0; i < 7; ++i)
        c.coeff[i] = (a.coeff[i] + b.coeff[i]) % 3;
    return c;
}

void poly_print(poly_t p) {
    for (int i = 6; i >= 0; --i)
        printf("%+dx^%d ", p.coeff[i], i);
    printf("\n");
}

int main() {
    poly_t k, m, c;
    printf("Enter coeffs of k(x): ");
    poly_read(&k);
    poly_print(k);

    printf("Enter coeffs of m(x): ");
    poly_read(&m);
    poly_print(m);

    printf("c(x): ");
    c = poly_add(k, m);
    poly_print(c);

    return 0;
}
