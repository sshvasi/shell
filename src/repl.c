#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "repl.h"

#define BUFFER_SIZE 64

char buffer[BUFFER_SIZE];

enum state {
    normal,
    quote,
    escape
};

static void repl_print_prompt()
{
    fputs("> ", stdout);
    fflush(stdout);
}

static void repl_read_line()
{
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            fputs("Cannot read a line.\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
}

static bool isdoublequote(int ch) { return ch == '"'; }

static bool isescape(int ch) { return ch == '\\'; }

static void repl_parse_line(struct node *list)
{
    struct node *first_word, *last_word;
    int word_index, buff_index;
    enum state curr_state;

    first_word = last_word = list;
    buff_index = word_index = 0;
    curr_state = normal;

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
                    list_add(&last_word);
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
        list_add(&last_word);
    }

    list_term(first_word, curr_state == normal);
}

void repl_loop(struct node *list)
{
    while (true) {
        repl_print_prompt();
        repl_read_line();
        repl_parse_line(list);
    }
}
