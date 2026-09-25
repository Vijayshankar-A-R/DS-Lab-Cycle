#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define HASH_IMPLEMENTATION
#include "hash.h"

#define LEN 64

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

void string_free(void *a) {
    free(*(char **)a);
}

int main(int argc, char **argv) {
    assert(argc == 3 && "Usage: ./a.out 'vocabulary.txt' 'document.txt'");

    hash_t dict;
    ht_init(&dict, sizeof(char *), sizeof(bool), 29, string_free, NULL);
    set_hash(&dict, string_hash);
    set_cmp(&dict, string_cmp);
    
    FILE *fh;
    char line[LEN];
    // fill dictionary
    fh = fopen(argv[1], "r");
    assert(fh);
    
    while (fgets(line, LEN, fh)) {
        line[strcspn(line, "\n")] = '\0';
        char *s = strdup(line);
        bool b = true;
        ht_put(&dict, &s, &b);
    }

    fclose(fh);
    // now read and count mistakes
    int c = 0;
    fh = fopen(argv[2], "r");
    assert(fh);

    while (fgets(line, LEN, fh)) {
        line[strcspn(line, "\n")] = '\0';
        char *s = strdup(line);
        if (!ht_get(&dict, &s, NULL)) {
            c++;
        }
        free(s);
    }

    fclose(fh);
    printf("Mistakes: %d\n", c);
    return 0;
}
