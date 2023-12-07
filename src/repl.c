#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "repl.h"

#define BUFFER_SIZE 64

struct context {
    char buffer[BUFFER_SIZE];

    struct node *first_nd;
    struct node *last_nd;

    int word_idx;
    int buff_idx;
} ctx;

enum state {
    normal,
    quote,
    escape
} prev_st = normal, curr_st = normal;

enum event {
    quote_ch = '"',
    escape_ch = '\\',
    space_ch = ' ',
    tab_ch = '\t',
    nl_ch = '\n'
    /* default = any character */
} prev_ev, curr_ev;

typedef enum state (*processor)(enum event);

static void repl_print_prompt(void);
static void repl_read_line(void);
static void repl_parse_line(void);

static void process(enum event ev);

static enum state process_normal(enum event ev);
static enum state process_quote(enum event ev);
static enum state process_escape(enum event ev);

void repl_loop()
{
    while (true) {
        repl_print_prompt();
        repl_read_line();
        repl_parse_line();
    }
}

static void repl_print_prompt(void)
{
    fputs("> ", stdout);
    fflush(stdout);
}

static void repl_read_line(void)
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

static void repl_parse_line(void)
{
    ctx.first_nd = ctx.last_nd = list_init();
    ctx.word_idx = 0;

    for (ctx.buff_idx = 0;
         ctx.buff_idx < BUFFER_SIZE && ctx.buffer[ctx.buff_idx];
         ctx.buff_idx++) {
        process(ctx.buffer[ctx.buff_idx]);
    }

    if (ctx.word_idx > 0) {
        ctx.last_nd->word[ctx.word_idx] = '\0';
        list_add(&ctx.last_nd);
    }

    list_term(ctx.first_nd, curr_st == normal);
}

static void process(enum event ev)
{
    const processor processors[] = {
        process_normal,
        process_quote,
        process_escape
    };

    prev_ev = curr_ev;
    curr_ev = ev;

    enum state tmp_st = curr_st;
    curr_st = processors[curr_st](ev);
    prev_st = tmp_st;
}

static enum state process_normal(enum event ev)
{
    switch (ev) {
        case quote_ch:
            return quote;
        case escape_ch:
            return escape;
        case space_ch:
        case tab_ch:
        case nl_ch:
            if (ctx.word_idx > 0) {
                ctx.last_nd->word[ctx.word_idx] = '\0';
                ctx.word_idx = 0;
                list_add(&ctx.last_nd);
            }
            return normal;
        default:
            ctx.last_nd->word[ctx.word_idx++] = ev;
            return normal;
    }
}

static enum state process_quote(enum event ev)
{
    switch (ev) {
        case quote_ch:
            if (prev_ev == quote_ch)
                ctx.last_nd->word[ctx.word_idx++] = ' ';
            return normal;
        case escape_ch:
            return escape;
        case space_ch:
        case tab_ch:
        case nl_ch:
        default:
            ctx.last_nd->word[ctx.word_idx++] = ev;
            return quote;
    }
}

static enum state process_escape(enum event ev)
{
    switch (ev) {
        case quote_ch:
        case escape_ch:
        case space_ch:
        case tab_ch:
        case nl_ch:
        default:
            ctx.last_nd->word[ctx.word_idx++] = ev;
            return prev_st;
    }
}
