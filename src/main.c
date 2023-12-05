#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 64

int main(void)
{
    char buffer[BUFFER_SIZE];
    char *word;

    int word_index;
    int buff_index;

    bool space;

    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            if (feof(stdin)) {
                exit(EXIT_SUCCESS);
            } else {
                fputs("Cannot read a line.\n", stderr);
                exit(EXIT_FAILURE);
            }
        }

        for (buff_index = word_index = 0,
             space = false,
             word = malloc(BUFFER_SIZE);
             buff_index < strlen(buffer);
             buff_index++) {
            switch (buffer[buff_index]) {
                case '\n':
                    word[word_index] = '\0';
                    word_index = 0;
                    space = false;
                    printf("[%s]\n", word);
                    free(word);
                    word = malloc(BUFFER_SIZE);
                    break;
                case '\t':
                case ' ':
                    if (!space) {
                        word[word_index] = '\0';
                        word_index = 0;
                        space = true;
                        printf("[%s]\n", word);
                        free(word);
                        word = malloc(BUFFER_SIZE);
                    }
                    break;
                default:
                    if (space)
                        space = false;
                    word[word_index++] = buffer[buff_index];
                    break;
            }
        }
    }

    free(word);
    exit(EXIT_SUCCESS);
}
