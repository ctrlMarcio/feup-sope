#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int reps = atoi(argv[argc - 1]);

	for (int j = 0; j < reps; j++){
		printf("Hello");
		for (int i = 1; i < argc - 1; i++) {
			printf(" %s", argv[i]);
		}
		printf("!\n");
	}
	return 0;
}
