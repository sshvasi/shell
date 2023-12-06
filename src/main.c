#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 64

struct word_item {
    char *word;
    struct word_item *next;
};

int main(void) {
    char buffer[BUFFER_SIZE];

    struct word_item *first_word;
    struct word_item *new_word;
    struct word_item *tmp_word;

    int word_index;
    int buff_index;

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

        new_word = malloc(sizeof(struct word_item));
        new_word->word = malloc(BUFFER_SIZE);
        new_word->next = NULL;
        first_word = new_word;

        for (buff_index = word_index = 0;
             buff_index < strlen(buffer);
             buff_index++) {
            switch (buffer[buff_index]) {
                case '\n':
                case '\t':
                case ' ':
                    if (word_index == 0) {
                        break;
                    }

                    first_word->word[word_index] = '\0';
                    word_index = 0;

                    new_word = malloc(sizeof(struct word_item));
                    new_word->word = malloc(BUFFER_SIZE);
                    new_word->next = first_word;
                    first_word = new_word;
                    break;
                default:
                    first_word->word[word_index++] = buffer[buff_index];
            }
        }

        while (first_word) {
            if (strlen(first_word->word) > 0) {
                printf("[%s]\n", first_word->word);
            } else {
                puts("<empty word>");
            }

            tmp_word = first_word->next;
            free(first_word->word);
            free(first_word);
            first_word = tmp_word;
        }
    }

    exit(EXIT_SUCCESS);
}
