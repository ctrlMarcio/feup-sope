#include <stdio.h>

int main(int argc, char *argv[], char *argenv[]) {
	while (*argenv) {
		printf("%s\n", *argenv++);
	}

	return 0;
}
