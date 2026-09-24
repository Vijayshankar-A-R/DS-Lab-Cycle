#include <stdio.h>
#include <string.h>
#include "linklist.h"

#define LEN	64

typedef struct {
	char app_name[LEN];
} app_t;

int main() {

	linklist_t freq_used;
	ll_init(&freq_used, sizeof(app_t), NULL);

	int c, f, r, i;
	char tmp[LEN];
	app_t app;
	r = 1;
	do {
		printf(	"1. Open\n"
			"2. Clean\n"
			"3. Exit\n\n");
		scanf("%d", &c);
		getc(stdin);

		switch (c) {
			case 1:
				printf("Enter name of app to open: ");
				fgets(tmp, LEN, stdin);
				tmp[strcspn(tmp, "\n")] = '\0';
				f = 0;
				i = 0;
				for (ll_node *cur = freq_used.head; cur; cur = cur->next, i++) {
					app = *(app_t *)cur->data;
					if (!strcmp(tmp, app.app_name)) {
						ll_delete(&freq_used, i, &app);
						ll_inserthead(&freq_used, &app);
						f = 1;
					}
				}
				if (!f) {
					strcpy(app.app_name, tmp);
					ll_inserthead(&freq_used, &app);
				}
				memset(tmp, 0, LEN);
				break;
			case 2:
				i = ll_deletetail(&freq_used, &app);
				if (i) printf("Deleted app: %s\n", app.app_name);
				else printf("Empty\n");
				break;
			case 3:
				r = 0;
				break;
			default:
				printf("Invalid option\n");
				break;
		}
	} while (r);

	ll_free(&freq_used);

	return 0;
}
