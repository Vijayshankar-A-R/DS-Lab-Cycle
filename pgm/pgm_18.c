#include <alloca.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifdef COUNT_TOT
#define COUNT_CMP
#define COUNT_SHF
#endif

#if !defined(COUNT_CMP) && !defined(COUNT_SHF)
#error "You are not counting anything! Define COUNT_CMP, COUNT_SHF or COUNT_TOT..."
#endif

// define COUNT_CMP to count comparisons
// define COUNT_SHF to count shifts

// Due to the ambiguousity of the question on the
// definition of a 'step', you may notice double counting or
// even triple counting based on the particular definition
// adopted. But the overall growth should still be reflected
// as the differences are only linear

int cmp(int a, int b, int *s) {
#ifdef COUNT_CMP
    (*s)++;
#endif
    return a - b;
}

void swp(int *a, int *b, int *s) {
#ifdef COUNT_SHF
    (*s) += 3;
#endif
    int t = *a;
    *a = *b;
    *b = t;
}

void shift(int *a, int b, int *s) {
#ifdef COUNT_SHF
    (*s)++;
#endif
    *a = b;
}

int bubble_sort(int *a, int n) {
    int c = 0;

    for (int i = 0; i < n; ++i) {
        int s = 0;

        for (int j = 0; j < n - i - 1; ++j) {
            if (cmp(a[j], a[j + 1], &c) < 0) {
                swp(&a[j], &a[j + 1], &c);
                s = 1;
            }
        }
        if (!s)
            break;
    }

    return c;
}

int insertion_sort(int *a, int n) {
    if (n < 2)
        return 0;
    int c = 0;

    for (int i = 1; i < n; ++i) {
        int k = a[i];
        int j;
        for (j = i - 1; j >= 0 && cmp(a[j], k, &c) < 0; --j)
            shift(&a[j + 1], a[j], &c);
        shift(&a[j + 1], k, &c);
    }

    return c;
}

int count_sort_by_digit(int *a, int n, int place) {
    int c = 0;

    int out[n];
    int cnt[10] = {0};

    for (int i = 0; i < n; ++i) {
        int d;
        shift(&d, 9 - (a[i] / place) % 10, &c);
        cnt[d]++;
    }

    for (int i = 1; i < 10; ++i)
        cnt[i] += cnt[i - 1];

    for (int i = n - 1; i >= 0; --i) {
        int d;
        shift(&d, 9 - (a[i] / place) % 10, &c);
        shift(&out[cnt[d] - 1], a[i], &c);
        cnt[d]--;
    }

    for (int i = 0; i < n; ++i)
        shift(&a[i], out[i], &c);
    return c;
}

int radix_sort(int *a, int n) {
    if (n == 0)
        return 0;
    int c = 0;

    int max = a[0];
    for (int i = 1; i < n; ++i)
        max = cmp(max, a[i], &c) > 0 ? max : a[i];

    int place = 1;
    while (max / place) {
        c += count_sort_by_digit(a, n, place);
        place *= 10;
    }

    return c;
}

int merge(int *a, int l, int m, int r) {
    int c = 0;
    int o, p;
    o = m - l + 1;
    p = r - m;
    int *lp = alloca(sizeof(int) * o);
    int *rp = alloca(sizeof(int) * p);

    for (int i = 0; i < o; ++i)
        shift(&lp[i], a[l + i], &c);
    for (int i = 0; i < p; ++i)
        shift(&rp[i], a[m + 1 + i], &c);

    int i, j, k;
    i = j = k = 0;
    while (i < o && j < p) {
        if (cmp(lp[i], rp[j], &c) > 0) {
            shift(&a[l + k], lp[i], &c);
            i++;
        } else {
            shift(&a[l + k], rp[j], &c);
            j++;
        }
        k++;
    }

    while (i < o) {
        shift(&a[l + k], lp[i], &c);
        i++;
        k++;
    }

    while (j < p) {
        shift(&a[l + k], rp[j], &c);
        j++;
        k++;
    }

    return c;
}

int merge_sort(int *a, int l, int r) {
    int c = 0;

    if (l >= r)
        return 0;

    int m = (l + r) / 2;

    c += merge_sort(a, l, m);
    c += merge_sort(a, m + 1, r);

    c += merge(a, l, m, r);
    return c;
}

int partition(int *a, int l, int h, int *c) {
    int p;
    shift(&p, a[h], c);
    int i = l - 1;

    for (int j = l; j < h; ++j) {
        if (cmp(a[j], p, c) > 0) {
            ++i;
            swp(&a[i], &a[j], c);
        }
    }
    swp(&a[i + 1], &a[h], c);
    return i + 1;
}

int quick_sort(int *a, int l, int h) {
    if (l >= h)
        return 0;
    int c = 0;
    int p = partition(a, l, h, &c);
    c += quick_sort(a, l, p - 1);
    c += quick_sort(a, p + 1, h);
    return c;
}

int main(int argc, char **argv) {
    assert(argc == 2 && "./a.out 'unsorted.txt'");
    int n = 0;
    FILE *fh = fopen(argv[1], "r");
    char line[8];
    while (fgets(line, 8, fh)) n++;
    int *a = alloca(sizeof(int) * n);
    int *b = alloca(sizeof(int) * n);
    rewind(fh);
    int i = 0;
    while (fgets(line, 8, fh)) {
        sscanf(line, "%d\n", &a[i++]);
    }
    fclose(fh);
    int s;
    
    //printf("original: ");
    //for (i = 0; i < n; ++i) printf("%d ", a[i]);
    //printf("\n");

#ifdef COUNT_CMP
    printf("Counting Comparisons...\n");
#endif
#ifdef COUNT_SHF
    printf("Counting Shifts...\n");
#endif

    memcpy(b, a, n * sizeof(int));
    s = bubble_sort(b, n);
    printf("bubble sort: ");
    //for (i = 0; i < n; ++i) printf("%d ", b[i]);
    printf("\nsteps: %d\n\n", s);

    memcpy(b, a, n * sizeof(int));
    s = insertion_sort(b, n);
    printf("insertion sort: ");
    //for (i = 0; i < n; ++i) printf("%d ", b[i]);
    printf("\nsteps: %d\n\n", s);

    memcpy(b, a, n * sizeof(int));
    s = radix_sort(b, n);
    printf("radix sort: ");
    //for (i = 0; i < n; ++i) printf("%d ", b[i]);
    printf("\nsteps: %d\n\n", s);

    memcpy(b, a, n * sizeof(int));
    s = merge_sort(b, 0, n - 1);
    printf("merge sort: ");
    //for (i = 0; i < n; ++i) printf("%d ", b[i]);
    printf("\nsteps: %d\n\n", s);

    memcpy(b, a, n * sizeof(int));
    s = quick_sort(b, 0, n - 1);
    printf("quick sort: ");
    //for (i = 0; i < n; ++i) printf("%d ", b[i]);
    printf("\nsteps: %d\n\n", s);

    return 0;
}
