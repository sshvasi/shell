#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "repl.h"

#define BUFFER_SIZE 64

enum state {
    normal,
    quote,
    escape
};

struct context {
    char buffer[BUFFER_SIZE];

    int prev_ch;
    int curr_ch;

    enum state prev_st;
    enum state curr_st;

    struct node *first_nd;
    struct node *last_nd;

    int word_idx;
    int buff_idx;
} ctx;

static void repl_print_prompt()
{
    fputs("> ", stdout);
    fflush(stdout);
}

static void repl_read_line()
{
    if (fgets(ctx.buffer, BUFFER_SIZE, stdin) == NULL) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            fputs("Cannot read a line.\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
}

static void repl_to_state(enum state st)
{
    ctx.prev_st = ctx.curr_st;
    ctx.curr_st = st;
}

static void repl_process_normal(int ch)
{
    if (ctx.curr_ch == '"') {
        repl_to_state(quote);
    } else if (ctx.curr_ch == '\\') {
        repl_to_state(escape);
    } else if (!isspace(ctx.curr_ch)) {
        ctx.last_nd->word[ctx.word_idx++] = ctx.curr_ch;
    } else if (ctx.word_idx > 0) {
        ctx.last_nd->word[ctx.word_idx] = '\0';
        ctx.word_idx = 0;
        list_add(&ctx.last_nd);
    }
}

static void repl_process_quote(int ch)
{
    if (ctx.curr_ch == '"') {
        if (ctx.prev_ch == '"') {
            ctx.last_nd->word[ctx.word_idx++] = ' ';
        }
        repl_to_state(normal);
    } else if (ctx.curr_ch == '\\') {
        repl_to_state(escape);
    } else {
        ctx.last_nd->word[ctx.word_idx++] = ctx.curr_ch;
    }
}

static void repl_process_escape(int ch)
{
    ctx.last_nd->word[ctx.word_idx++] = ctx.curr_ch;
    repl_to_state(ctx.prev_st);
}

static void repl_process(int ch)
{
    ctx.curr_ch = ch;
    switch (ctx.curr_st) {
        case normal:
            repl_process_normal(ch);
            break;
        case quote:
            repl_process_quote(ch);
            break;
        case escape:
            repl_process_escape(ch);
            break;
        default:
            break;

    }
    ctx.prev_ch = ctx.curr_ch;
}

static void repl_parse_line()
{
    ctx.first_nd = ctx.last_nd = list_init();
    ctx.curr_st = ctx.prev_st = normal;
    ctx.word_idx = 0;

    for (ctx.buff_idx = 0;
         ctx.buff_idx < BUFFER_SIZE && ctx.buffer[ctx.buff_idx];
         ctx.buff_idx++) {
        repl_process(ctx.buffer[ctx.buff_idx]);
    }

    if (ctx.word_idx > 0) {
        ctx.last_nd->word[ctx.word_idx] = '\0';
        list_add(&ctx.last_nd);
    }

    list_term(ctx.first_nd, ctx.curr_st == normal);
}

void repl_loop()
{
    while (true) {
        repl_print_prompt();
        repl_read_line();
        repl_parse_line();
    }
}
