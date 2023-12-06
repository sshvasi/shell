#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 64

struct node {
    char *word;
    struct node *next;
};

static struct node *init_list(void);
static void add_to_list(struct node **tail);
static void term_list(struct node*);

int main(void) {
    char buffer[BUFFER_SIZE];

    struct node *first_word;
    struct node *last_word;

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

        for (first_word = last_word = init_list(),
             buff_index = word_index = 0;
             buff_index < strlen(buffer);
             buff_index++) {
            switch (buffer[buff_index]) {
                case '\n':
                case '\t':
                case ' ':
                    if (word_index == 0) {
                        break;
                    }

                    last_word->word[word_index] = '\0';
                    word_index = 0;

                    add_to_list(&last_word);
                    break;
                default:
                    last_word->word[word_index++] = buffer[buff_index];
            }
        }

        term_list(first_word);
    }

    exit(EXIT_SUCCESS);
}

static struct node *init_list(void)
{
    struct node *head = malloc(sizeof(struct node));
    head->word = malloc(BUFFER_SIZE);
    head->next = NULL;

    return head;
}

static void add_to_list(struct node **tail)
{
    struct node *new_tail = malloc(sizeof(struct node));
    new_tail->word = malloc(BUFFER_SIZE);
    new_tail->next = NULL;

    (*tail)->next = new_tail;
    *tail = new_tail;
}

static void term_list(struct node *head)
{
    while (head) {
        if (strlen(head->word) > 0) {
            printf("[%s]\n", head->word);
        } else {
            puts("<empty word>");
        }

        struct node *tmp = head->next;
        free(head->word);
        free(head);
        head = tmp;
    }
}
