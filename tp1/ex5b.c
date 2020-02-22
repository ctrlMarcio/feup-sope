#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int agrc, char *argv[], char *argenv[]) {
	printf("Hello ");
	char name[32];

	while(*argenv) {
		if (strncmp(*argenv, "USER=", 5) == 0) {
			memcpy(name, *argenv + 5, 32);
			name[31] = '\0';
			printf("%s!\n", name);
			break;	
		}

		argenv++;
	}

	return 0;
}
