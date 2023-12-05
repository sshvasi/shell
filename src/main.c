#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 64

int main(void)
{
    char buffer[BUFFER_SIZE];

    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            if (feof(stdin)) {
                exit(EXIT_SUCCESS);
            } else {
                fprintf(stderr, "Cannot read a line.");
                exit(EXIT_FAILURE);
            }
        }

        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }

        for (int i = 0; i < strlen(buffer); i++) {
            switch (buffer[i]) {
                case '\t':
                case ' ':
                    break;
                case '"':
                    break;
                default:
                    printf("[%c]\n", buffer[i]);
                    break;
            }
        }
    }

    exit(EXIT_SUCCESS);
}
