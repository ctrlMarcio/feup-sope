#include <stdio.h>
#include <string.h>

#define MAX_BUFFER 100
#define MAX_WORDS 15

void getTokens(char *dst[], char *src) {
    char *token;
    token = strtok(src, " ");

    int i;
    for (i = 0; i < MAX_WORDS && token != NULL; ++i) {
        dst[i] = token;
        token = strtok(NULL, " ");
    }
    dst[i] = NULL;
}

int main() {
    char input[MAX_BUFFER];
    char *tokens[MAX_WORDS];

    fputs("$ ", stdout);
    fgets(input, MAX_BUFFER, stdin);

    getTokens(tokens, input);

    for (int j = 0; j < MAX_WORDS; j++) {
        if (!tokens[j])
            break;

        printf("%s\n", tokens[j]);    
    }

    return 0;
}