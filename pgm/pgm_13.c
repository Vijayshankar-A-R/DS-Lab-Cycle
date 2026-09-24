#include <stdio.h>
#include <string.h>
#define BST_IMPLEMENTATION
#include "bst.h"

#define W_LEN 32
#define M_LEN 256

struct word_meaning_pair {
    char *word;
    char *meaning;
};

typedef struct word_meaning_pair dict_entry_t;
typedef bst_t dict_t;

void create_entry(dict_entry_t *e, const char *word, const char *meaning) {
    e->word = strdup(word);
    e->meaning = strdup(meaning);
}

void free_entry(void *a) {
    dict_entry_t *e = (dict_entry_t *)a;
    free(e->word);
    free(e->meaning);
}

int cmp_entry(const void *a, const void *b) {
    const dict_entry_t *e1, *e2;
    e1 = (const dict_entry_t *)a;
    e2 = (const dict_entry_t *)b;
    return strcmp(e1->word, e2->word);
}

void dict_init(dict_t *d) {
    bst_init(d, sizeof(dict_entry_t), cmp_entry, free_entry);
}

const char *get_meaning(const dict_t *d, char *word) {
    dict_entry_t e;
    dict_entry_t k = {.word = word};
    int s = bst_get(d, &k, &e);
    return s ? e.meaning : "Not found";
}

void insert_meaning(dict_t *d, const char *word, const char *meaning) {
    dict_entry_t e;
    create_entry(&e, word, meaning);
    bst_insert(d, &e);
}

void remove_meaning(dict_t *d, char *word) {
    dict_entry_t k;
    k.word = word;
    bst_delete(d, &k);
}

int main() {
    dict_t dict;
    dict_init(&dict);

    printf("1. Insert meaning\n"
           "2. Get meaning\n"
           "3. Remove meaning\n"
           "4. Exit\n\n");
    int r = 1;
    int c;
    char w[W_LEN], m[M_LEN];
    do {
        scanf("%d", &c);
        getc(stdin);

        switch(c) {
        case 1:
            printf("Word: ");
            fgets(w, W_LEN, stdin);
            w[strcspn(w, "\n")] = '\0';
            
            printf("Meaning: ");
            fgets(m, M_LEN, stdin);
            m[strcspn(m, "\n")] = '\0';

            insert_meaning(&dict, w, m);
            break;
        case 2:
            printf("Word: ");
            fgets(w, W_LEN, stdin);
            w[strcspn(w, "\n")] = '\0';

            printf("Meaning: %s\n", get_meaning(&dict, w));
            break;
        case 3:
            printf("Word: ");
            fgets(w, W_LEN, stdin);
            w[strcspn(w, "\n")] = '\0';

            remove_meaning(&dict, w);
            break;
        case 4:
            r = 0;
            break;
        default:
            printf("Invalid option\n");
            break;
        }
    } while(r);
    return 0;
}
