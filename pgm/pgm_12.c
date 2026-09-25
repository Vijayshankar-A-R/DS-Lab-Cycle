#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define STACK_IMPLEMENTATION
#include "stack.h"

#define BUFFER_LEN 64

typedef struct tree_node {
    char val;
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *self; // reference to original allocated mem
} node_t;

void create_node(node_t *n, char val) {
    n->val = val;
    n->left = NULL;
    n->right = NULL;
    n->self = n;
}

void print_tree(node_t *n, int d) {
    if (!n)
        return;
    for (int i = 0; i < d; ++i)
        printf("    ");
    printf("%c\n", n->val);
    print_tree(n->left, d + 1);
    print_tree(n->right, d + 1);
}

void free_tree(node_t *n) {
    if (!n)
        return;
    free_tree(n->left);
    free_tree(n->right);
    free(n->self);
}

int precedence(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return 0; // shouldn't be reached
    }
}

void itopfix(const char infix[BUFFER_LEN], char res[BUFFER_LEN]) {
    stack_t cstck; // stack of operator
    int i = 0;     // current string index
    stack_init(&cstck, sizeof(char), NULL);

    for (int j = 0; infix[j] != '\0'; ++j) {
        char c = infix[j];
        if (isspace(c))
            continue;

        if (isalnum(c))
            res[i++] = c;

        else if (c == '(')
            stack_push(&cstck, &c);

        else if (c == ')') {
            char d;
            stack_peek(&cstck, &d);
            while (d != '(') {
                stack_pop(&cstck, &d);
                res[i++] = d;
                stack_peek(&cstck, &d);
            }
            stack_pop(&cstck, &d); // pop '('
        }
        // Assume invalid characters will not be entered
        // Assume no variables
        else {
            char d;
            if (!stack_isempty(&cstck)) {
                stack_peek(&cstck, &d);
                while (precedence(d) >= precedence(c)) {
                    stack_pop(&cstck, &d);
                    res[i++] = d;
                    if (stack_isempty(&cstck))
                        break;
                    stack_peek(&cstck, &d);
                }
            }
            stack_push(&cstck, &c);
        }
    }
    while (!stack_isempty(&cstck)) {
        char d;
        stack_pop(&cstck, &d);
        res[i++] = d;
    }
    res[i] = '\0'; // terminate the string
    stack_free(&cstck);
}

node_t *peval(char postfix[BUFFER_LEN]) {
    stack_t dstck;
    stack_init(&dstck, sizeof(node_t), NULL);

    for (int i = 0; postfix[i] != '\0'; ++i) {
        char c = postfix[i];
        if (isalnum(c)) {
            node_t *a = malloc(sizeof(node_t));
            create_node(a, c);
            stack_push(&dstck, a);
        }
        // Operator otherwise
        else {
            // Only binary operators supported
            node_t a, b, *d;
            d = malloc(sizeof(node_t));
            stack_pop(&dstck, &b);
            stack_pop(&dstck, &a);
            create_node(d, c);
            d->left = a.self;
            d->right = b.self;
            stack_push(&dstck, d);
        }
    }
    node_t r;
    stack_pop(&dstck, &r);
    stack_free(&dstck);
    return r.self;
}

int main() {
    char infix[BUFFER_LEN] = {0};
    char postfix[BUFFER_LEN] = {0};
    int c, r;
    r = 1;
    printf("1. Infix to Postfix\n"
           "2. Evaluate stored postfix\n"
           "3. Exit\n");
    do {
        printf("Option: ");
        scanf("%d", &c);
        getc(stdin);
        switch (c) {
        case 1:
            printf("Enter infix: ");
            fgets(infix, BUFFER_LEN - 1, stdin);
            itopfix(infix, postfix);
            printf("Stored postfix: %s\n", postfix);
            break;
        case 2:
            printf("Evaluating %s ( %s )\n", infix, postfix);
            printf("Result: \n");
            node_t *res = peval(postfix);
            print_tree(res, 0);
            free_tree(res);
            break;
        case 3:
            r = 0;
            break;
        default:
            printf("Invalid option\n");
            break;
        }
    } while (r);
    return 0;
}
