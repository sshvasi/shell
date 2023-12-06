#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "repl.h"

#define BUFFER_SIZE 64

char buffer[BUFFER_SIZE];

enum state { normal, quote, escape };

static bool isdoublequote(int ch) { return ch == '"'; }

static bool isescape(int ch) { return ch == '\\'; }

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

static void repl_parse_line()
{
    struct node *first_word, *last_word;
    int word_index, buff_index;
    enum state curr_state, prev_state;
    char curr_char, prev_char;

    first_word = last_word = list_init();
    curr_state = prev_state = normal;
    word_index = 0;

    for (buff_index = 0;
         buff_index < BUFFER_SIZE && buffer[buff_index] != '\0';
         buff_index++) {
        curr_char = buffer[buff_index];

        switch (curr_state) {
            case normal:
                if (isdoublequote(curr_char)) {
                    curr_state = quote;
                } else if (isescape(curr_char)) {
                    prev_state = curr_state;
                    curr_state = escape;
                } else if (!isspace(curr_char)) {
                    last_word->word[word_index++] = curr_char;
                } else if (word_index > 0) {
                    last_word->word[word_index] = '\0';
                    word_index = 0;
                    list_add(&last_word);
                }
                break;
            case quote:
                if (isdoublequote(curr_char)) {
                    if (isdoublequote(prev_char)) {
                        last_word->word[word_index++] = ' ';
                    }
                    curr_state = normal;
                } else if (isescape(curr_char)) {
                    prev_state = curr_state;
                    curr_state = escape;
                } else {
                    last_word->word[word_index++] = curr_char;
                }
                break;
            case escape:
                last_word->word[word_index++] = curr_char;
                curr_state = prev_state;
                break;
            default:
                break;
        }

        prev_char = curr_char;
    }

    if (word_index > 0) {
        last_word->word[word_index] = '\0';
        list_add(&last_word);
    }

    list_term(first_word, curr_state == normal);
}

void repl_loop()
{
    while (true) {
        repl_print_prompt();
        repl_read_line();
        repl_parse_line();
    }
}
