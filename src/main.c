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

        if ((fgets(buffer, sizeof(buffer), stdin)) == NULL) {
            if (feof(stdin)) {
                return EXIT_SUCCESS;
            } else {
                fprintf(stderr, "Cannot read a line.");
                return EXIT_FAILURE;
            }
        }

        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }

        printf("[%s]\n", buffer);
    }

    return EXIT_SUCCESS;
}
