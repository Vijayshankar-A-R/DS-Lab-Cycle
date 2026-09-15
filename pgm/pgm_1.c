#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

bool is_feasible(const int *books, int n, int s, int max) {
    // Returns if it is possibile to distribute n books to s children such that pages alloted for each children <=max
    int curr = 0;
    for (int i = 0; i < n; ++i) {
        if (curr + books[i] > max) {
            s--;
            if (!s) return false;
            curr = books[i];
        } else {
            curr += books[i];
        }
    }
    return true;
}

int main() {
    int n, s, c, max, sum;
    printf("#Books: ");
    scanf("%d", &n);
    int arr[n];
    max = INT_MIN;
    sum = 0;
    for (int i = 0; i < n; ++i) {
        printf("Pages in book-%d: ", i + 1);
        scanf("%d", &c);
        arr[i] = c;
        max = c > max ? c : max;
        sum += c;
    }
    printf("#Students: ");
    scanf("%d", &s);

    // Binary search: the minimum max_pages lies between max(books) and sum(books)
    // All value >= the minimum return true on feasibility check and the rest don't
    int l, r, m;
    l = max;
    r = sum;
    while (l < r) {
        m = (l + r) / 2;
        
        if (is_feasible(arr, n, s, m)) r = m;
        else l = m + 1;

    }
    
    // Our answer is in m
    // Reconstruct the book allocation
    printf("Maximum: %d\n", m);
    int curr = 0;
    int st = 1;
    printf("Student-1: ");
    for (int i = 0; i < n; ++i) {
        if (curr + arr[i] > m) {
            st++;
            printf("\nStudent-%d: %d ", st, arr[i]);
            curr = arr[i];
        } else {
            printf("%d ", arr[i]);
            curr += arr[i];
        }
    }
    printf("\n");

    return 0;
}
