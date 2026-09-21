#include <assert.h>
#include <stdio.h>
#include <string.h>
#define LIST_IMPLEMENTATION
#include "list.h"

#define LINE 1024
#define LEN 64

typedef char word_t[LEN];

void print_words(list_t words) {
    word_t w;
    for (int i = 0; i < list_size(&words); ++i) {
        get(&words, i, &w);
        printf("%s ", w);
    }
    printf("\n");
}

int replace_words(list_t words, word_t old, word_t new) {
    word_t w;
    int n = 0;
    for (int i = 0; i < list_size(&words); ++i) {
        get(&words, i, &w);
        if (!strcmp(w, old))
            set(&words, i, new) && n++;
    }
    return n;
}

int main(int argc, char **argv) {
    assert(argc == 3 && "Use: ./a.out 'old' 'new'\n");
    list_t words;
    list_init(&words, 20, LEN, NULL);
    char line[LINE];

    while (fgets(line, LINE, stdin) != NULL) {
        char *w;
        char delim[] = " \t\n\r";
        w = strtok(line, delim);
        while (w) {
            add(&words, w);
            w = strtok(NULL, delim);
        }
    }

    word_t old, new;
    int n;
    strncpy(old, argv[1], LEN);
    strncpy(new, argv[2], LEN);
    n = replace_words(words, old, new);
    printf("(%d replacements made)\n", n);
    print_words(words);

    return 0;
}
