#include <stdio.h>
#define LIST_IMPLEMENTATION
#include "list.h"
#define QUEUE_IMPLEMENTATION
#include "queue.h"
#define SET_IMPLEMENTATION
#include "set.h"

int min(int a, int b) { return a < b ? a : b; }

struct jug_state {
    int c10;
    int c7;
    int c4;
};

struct overall_state {
    struct jug_state js;
    struct overall_state *prev;
};

typedef struct jug_state state_t;

void rev(state_t *a, int n) {
    for (int i = 0; i < n / 2; ++i) {
        state_t t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}

list_t search_space(state_t init, int (*istarget)(const state_t)) {
    queue_t bfs;
    set_t visited;

    q_init(&bfs, sizeof(struct overall_state), NULL);
    set_init(&visited, sizeof(state_t), NULL);

    struct overall_state cur_state = {.js = init, .prev = NULL};
    enqueue(&bfs, &cur_state);
    while (!q_isempty(&bfs)) {
        state_t cur;
        dequeue(&bfs, &cur_state);
        cur = cur_state.js;
        if (set_contains(&visited, &cur))
            continue;
        if (istarget(cur))
            goto path_construct;
        set_insert(&visited, &cur);

        struct overall_state nxt_states[12];
        int amt;
        for (int i = 0; i < 6; ++i) {
            nxt_states[i].prev = malloc(sizeof(struct overall_state));
            *nxt_states[i].prev = cur_state;
            nxt_states[i].js = cur;
        }
        // Fills
        // nxt_states[0].js.c10 = 10;
        // nxt_states[1].js.c7 = 7;
        // nxt_states[2].js.c4 = 4;

        // Empty
        // nxt_states[3].js.c10 = 0;
        // nxt_states[4].js.c7 = 0;
        // nxt_states[5].js.c4 = 0;

        // Pour c10 to c7
        amt = min(cur.c10, 7 - cur.c7);
        nxt_states[0].js.c10 -= amt;
        nxt_states[0].js.c7 += amt;

        // Pour c10 to c4
        amt = min(cur.c10, 4 - cur.c4);
        nxt_states[1].js.c10 -= amt;
        nxt_states[1].js.c4 += amt;

        // Pour c7 to c10
        amt = min(cur.c7, 10 - cur.c10);
        nxt_states[2].js.c7 -= amt;
        nxt_states[2].js.c10 += amt;

        // Pour c7 to c4
        amt = min(cur.c7, 4 - cur.c4);
        nxt_states[3].js.c7 -= amt;
        nxt_states[3].js.c4 += amt;

        // Pour c4 to c10
        amt = min(cur.c4, 10 - cur.c10);
        nxt_states[4].js.c4 -= amt;
        nxt_states[4].js.c10 += amt;

        // Pour c4 to c7
        amt = min(cur.c4, 7 - cur.c7);
        nxt_states[5].js.c4 -= amt;
        nxt_states[5].js.c7 += amt;

        for (int i = 0; i < 6; ++i)
            enqueue(&bfs, &nxt_states[i]);
    }
    goto free_ret;

    list_t path;
path_construct:
    list_init(&path, 3, sizeof(state_t), NULL);
    do {
        add(&path, &cur_state.js);
        cur_state = *cur_state.prev;
    } while (cur_state.prev != NULL);
    add(&path, &cur_state.js);
    rev((state_t *)path.data, path.size);
free_ret:
    q_free(&bfs);
    set_free(&visited);
    return path;
}

int istarget(const state_t s) {
#ifdef ORIGINAL
    return s.c7 == 2 || s.c4 == 2;
#endif
#ifdef DBG1
    return s.c10 == 2 && s.c7 == 5 && s.c4 == 3; // unreachable
#endif
#ifdef DBG2
    return s.c10 == 8 && s.c7 == 3 && s.c4 == 0; // reachable
#endif
    return 0;
}

int main() {
    list_t result;
    state_t init = (state_t){.c10 = 0, .c7 = 7, .c4 = 4};
    result = search_space(init, istarget);

    printf(
#ifdef ORIGINAL
            "target = (X 2 X) or (X X 2)\n"
#endif
#ifdef DBG1
            "target = (2 5 3) (unreachable)\n"
#endif
#ifdef DBG2
            "target = (8 3 0) (reachable)\n"
#endif
            ""
          );

    for (int i = 0; i < list_size(&result); ++i) {
        state_t cur;
        get(&result, i, &cur);
        printf(" (%d %d %d) -> ", cur.c10, cur.c7, cur.c4); 
    }
    printf(list_size(&result) ? " !\n" : "X\n");
    return 0;
}
