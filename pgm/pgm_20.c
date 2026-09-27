#include <assert.h>
#include <stdio.h>
#include <string.h>
#define SET_IMPLEMENTATION
#include "set.h"
#define LIST_IMPLEMENTATION
#include "list.h"

#define LEN 64

int min(size_t a, size_t b, size_t c) {
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

// Levenshtein Distance
size_t edit_dist(const char *a, const char *b) {
    size_t m, n;
    m = strlen(a);
    n = strlen(b);

    size_t D[m + 1][n + 1];
    size_t i, j;
    for (j = 0; j <= n; ++j)
        D[0][j] = j;

    for (i = 0; i <= m; ++i)
        D[i][0] = i;

    size_t c;
    for (i = 1; i <= m; ++i) {
        for (j = 1; j <= n; ++j) {
            c = (a[i - 1] == b[j - 1]) ? 0 : 1;
            D[i][j] = min(
                    D[i - 1][j] + 1,    // delete
                    D[i][j - 1] + 1,    // insert
                    D[i - 1][j - 1] + c // substitute
                    );
        }
    }
    return D[m][n];
}

size_t string_hash(const void *a, size_t size) {
    size_t h = __hash(*(const char **)a, strlen(*(const char **)a));
    return h;
}

int string_cmp(const void *a, const void *b, size_t s) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    int r = strcmp(s1, s2);
    return r;
}

void string_free(void *a) { free(*(char **)a); }

int main(int argc, char **argv) {
    assert(argc == 3 && "Usage: ./a.out 'vocabulary.txt' 'document.txt'");

    set_t dict;
    list_t all_words;
    set_init(&dict, sizeof(char *), string_free);
    set_hash(&dict, string_hash);
    set_cmp(&dict, string_cmp);
    list_init(&all_words, 3, sizeof(char *), NULL); // set will free

    FILE *fh;
    char line[LEN];
    // fill dictionary
    fh = fopen(argv[1], "r");
    assert(fh);

    while (fgets(line, LEN, fh)) {
        line[strcspn(line, "\n")] = '\0';
        char *s = strdup(line);
        set_insert(&dict, &s);
        add(&all_words, &s);
    }

    fclose(fh);
    // now read and count mistakes
    int c = 0;
    fh = fopen(argv[2], "r");
    assert(fh);

    while (fgets(line, LEN, fh)) {
        line[strcspn(line, "\n")] = '\0';
        char *s = strdup(line);
        if (!set_contains(&dict, &s)) {
            char *min_word;
            get(&all_words, 0, &min_word);
            int min_dist = edit_dist(s, min_word);
            for (int i = 1; i < list_size(&all_words); ++i) {
                char *cur;
                int cur_dist;
                get(&all_words, i, &cur);
                cur_dist = edit_dist(s, cur);
                if (cur_dist < min_dist) {
                    min_word = cur;
                    min_dist = cur_dist;
                }
            }
            if (min_dist < 4) {
                printf("You typed '%s'. Did you mean '%s'?\n", s, min_word);
            }
            c++;
        }
        free(s);
    }

    set_free(&dict);
    list_free(&all_words);
    fclose(fh);
    printf("Mistakes: %d\n", c);
    return 0;
}
