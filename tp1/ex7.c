#include <stdlib.h>
#include <stdio.h>

void atexit1() {
	printf("Executing atexit 1\n");
}

void atexit2() {
	printf("Executing atexit 2\n");
}

int main() {
	atexit(atexit1);
	atexit(atexit2);

	printf("Main done\n");
	return 0;
}
