#include <ctype.h>
#include <math.h>
#include <stdio.h>
#define STACK_IMPLEMENTATION
#include "stack.h"

#define BUFFER_LEN 64

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

        if (isdigit(c))
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

int ctoi(char c) { return c - '0'; }

double peval(char postfix[BUFFER_LEN]) {
    stack_t dstck;
    stack_init(&dstck, sizeof(double), NULL);

    for (int i = 0; postfix[i] != '\0'; ++i) {
        char c = postfix[i];
        if (isdigit(c)) {
            double i = (double)ctoi(c); // Assume single digit integers
            stack_push(&dstck, &i);
        }
        // Operator otherwise
        else {
            // Only binary operators supported
            double a, b, d;
            stack_pop(&dstck, &b);
            stack_pop(&dstck, &a);
            switch (c) {
            case '+':
                d = a + b;
                break;
            case '-':
                d = a - b;
                break;
            case '*':
                d = a * b;
                break;
            case '/':
                d = a / b;
                break;
            case '^':
                d = pow(a, b);
                break;
            default:
                d = -1; // shouldn't reach here
                break;
            }
            stack_push(&dstck, &d);
        }
    }
    double r;
    stack_pop(&dstck, &r);
    stack_free(&dstck);
    return r;
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
            printf("Result: %g\n", peval(postfix));
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
