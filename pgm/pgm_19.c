#include <stdio.h>
#include <stdlib.h>

int main() {
	int n, k;
	printf("Enter max events participatable: ");
	scanf("%d", &k);
	printf("Enter number of events: ");
	scanf("%d", &n);
	int A[n];
	int *B = alloca(k * sizeof(int));
	
	for (int i = 0; i < n; ++i)
		scanf("%d", &A[i]);

	for (int i = 0; i < k; ++i)
		B[i] = 0;
	
	for (int i = 0; i < n; ++i) {
		int a = A[i];
		for (int j = 0; j < k; ++j) {
			if (a > B[j]) {
				for (int m = k - 1; m > j; --m)
					B[m] = B[m - 1];
				B[j] = a;
				break;
			}
		}
	}

	int sum = 0;
	for (int i = 0; i < k; ++i)
		sum += B[i];
	printf("\n\nsum = %d\n", sum);

	return 0;
}
