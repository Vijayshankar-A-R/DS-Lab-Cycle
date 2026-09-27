#include <stdio.h>
#include <stdlib.h> // abs
#include <limits.h>
#define HEAP_IMPLEMENTATION
#include "heap.h"
#define LIST_IMPLEMENTATION
#include "list.h"

#define MAP_VAL(map, pt) ((map)[(pt).x][(pt).y])

typedef struct {
    int x;
    int y;
} point_t;

struct heap_struct {
    point_t p;
    int score;
};

int heap_cmp(const void *a, const void *b) {
    return ((const struct heap_struct *)b)->score - ((const struct heap_struct *)a)->score; 
}

int dist(const point_t src, const point_t dst) {
    return abs(src.x - dst.x) + abs(src.y - dst.y);
}

int cmp(const point_t a, const point_t b) {
    return dist(a, b) == 0;
}

void rev(point_t *p, int n) {
    for (int i = 0; i < n / 2; ++i) {
        point_t t = p[i];
        p[i] = p[n - 1 - i];
        p[n - 1 - i] = t;
    }
}

int is_valid(int m, int n, int grid[m][n], point_t p) {
    if (p.x < 0 || p.x >= m) return 0;
    if (p.y < 0 || p.y >= n) return 0;
    return grid[p.x][p.y] != 1;
}

list_t a_star(int m, int n, int grid[m][n], point_t start, point_t goal) {
    hp_t open;
    hp_init(&open, sizeof(struct heap_struct), heap_cmp, NULL);

    point_t (*cameFrom)[n] = malloc(sizeof(point_t) * m * n);
    int (*gscore)[n] = malloc(sizeof(int) * m * n);
    int (*fscore)[n] = malloc(sizeof(int) * m * n);

    for(int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cameFrom[i][j] = (point_t) {.x = -1, .y = -1};
            gscore[i][j] = INT_MAX;
            fscore[i][j] = INT_MAX;
        }
    }

    MAP_VAL(gscore, start) = 0;
    MAP_VAL(fscore, start) = dist(start, goal);

    struct heap_struct cur = (struct heap_struct) {.p = start, .score = MAP_VAL(fscore, start)};
    hp_insert(&open, &cur);
    
    list_t path;
    list_init(&path, 3, sizeof(point_t), NULL);

    while (hp_peek(&open, NULL)) {
        hp_popmax(&open, &cur);
        point_t cur_p = cur.p;
        if (cmp(cur_p, goal))
            goto path_reconstruct;
        point_t neighbours[4];

        neighbours[0].x = cur_p.x + -1;
        neighbours[0].y = cur_p.y + 0;

        neighbours[1].x = cur_p.x + 0;
        neighbours[1].y = cur_p.y + -1; 

        neighbours[2].x = cur_p.x + 0;
        neighbours[2].y = cur_p.y + 1; 

        neighbours[3].x = cur_p.x + 1;
        neighbours[3].y = cur_p.y + 0; 

        for (int i = 0; i < 4; ++i) {
            point_t neighbour = neighbours[i];
            if (!is_valid(m, n, grid, neighbour))
                continue;
            int tentative_gscore = MAP_VAL(gscore, cur_p) + 1;

            if (tentative_gscore < MAP_VAL(gscore, neighbour)) {
                MAP_VAL(cameFrom, neighbour) = cur_p;
                MAP_VAL(gscore, neighbour) = tentative_gscore;
                MAP_VAL(fscore, neighbour) = tentative_gscore + dist(neighbour, goal);

                struct heap_struct nxt = { .p = neighbour, .score = MAP_VAL(fscore, neighbour) };

                hp_insert(&open, &nxt);
            }

        }
    }
    goto free_ret;

    point_t cur_p;
path_reconstruct:
    cur_p = cur.p;
    add(&path, &cur_p);
    while (MAP_VAL(cameFrom, cur_p).x != -1) {
        cur_p = MAP_VAL(cameFrom, cur_p);
        add(&path, &cur_p);
    }
    rev((point_t *)path.data, path.size);
free_ret:
    free(gscore);
    free(fscore);
    free(cameFrom);
    hp_free(&open);
    return path;
}

int main() {
    int maze[8][8] = {
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 1, 0, 1, 0},
        {0, 1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 1, 0, 1}
    };
    point_t start = (point_t) {.x = 0, .y = 0};
    point_t goal = (point_t) {.x = 7, .y = 6};
    list_t result = a_star(8, 8, maze, start, goal);

    for (int i = 0; i < list_size(&result); ++i) {
        point_t c;
        get(&result, i, &c);
        printf(" (%d, %d) -> ", c.y, c.x);  // x and y got inverted;
    }
    printf(list_size(&result) ? "!\n" : "X\n");

    return 0;
}
