#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 64

enum state { normal, quote, escape };

struct node { char *word; struct node *next; };

static struct node *init_list(void);
static void add_to_list(struct node **tail);
static void term_list(struct node*, bool);

bool isdoublequote(int ch) { return ch == '"'; }
bool isescape(int ch) { return ch == '\\'; }

int main(void) {
    char buffer[BUFFER_SIZE];
    struct node *first_word, *last_word;
    int word_index, buff_index;
    enum state curr_state;

    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            if (feof(stdin)) {
                term_list(first_word, true);
                exit(EXIT_SUCCESS);
            } else {
                fputs("Cannot read a line.\n", stderr);
                term_list(first_word, true);
                exit(EXIT_FAILURE);
            }
        }

        first_word = last_word = init_list();
        curr_state = normal;
        word_index = 0;

        for (buff_index = 0;
             buff_index < BUFFER_SIZE && buffer[buff_index];
             buff_index++) {
            char ch = buffer[buff_index];

            switch (curr_state) {
                case normal:
                    if (isdoublequote(ch)) {
                        curr_state = quote;
                    } else if (!isspace(ch)) {
                        last_word->word[word_index++] = ch;
                    } else if (word_index > 0) {
                        last_word->word[word_index] = '\0';
                        word_index = 0;
                        add_to_list(&last_word);
                    }
                    break;
                case quote:
                    if (isdoublequote(ch)) {
                        curr_state = normal;
                    } else if (isescape(ch)) {
                        curr_state = escape;
                    } else {
                        last_word->word[word_index++] = ch;
                    }
                    break;
                case escape:
                    last_word->word[word_index++] = ch;
                    curr_state = quote;
                    break;
                default:
                    break;
            }
        }

        if (word_index > 0) {
            last_word->word[word_index] = '\0';
            add_to_list(&last_word);
        }

        term_list(first_word, curr_state == normal);
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

static void term_list(struct node *head, bool success)
{
    if (!success) {
        fputs("Syntax error. Incorrect number of quotes.\n", stderr);
    }

    while (head) {
        if (success) {
            if (strlen(head->word) > 0) {
                printf("[%s]\n", head->word);
            }
        }

        struct node *tmp = head->next;
        free(head->word);
        free(head);
        head = tmp;
    }
}
