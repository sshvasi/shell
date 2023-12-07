#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "repl.h"

struct list *list;
struct context ctx;
enum event prev_ev, curr_ev;
enum state prev_st = normal, curr_st = normal;

static enum state process_normal(enum event ev);
static enum state process_quote(enum event ev);
static enum state process_escape(enum event ev);

static void process(enum event ev);

static void repl_print_prompt(void);
static void repl_parse_line(void);
static void repl_read_line(void);

void init_repl()
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
            fputs("Failed to read a line.\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
}

static void repl_parse_line(void)
{
    list = init_list();
    ctx.word_idx = 0;

    for (ctx.buff_idx = 0;
         ctx.buff_idx < BUFFER_SIZE && ctx.buffer[ctx.buff_idx];
         ctx.buff_idx++) {
        process(ctx.buffer[ctx.buff_idx]);
    }
}

static void process(enum event ev)
{
    static const processor processors[] = {
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
        case nl_ch:
            ctx.word[ctx.word_idx] = '\0';
            ctx.word_idx = 0;
            add_to_list(list, ctx.word);
            print_list(list);
            free_list(list);
            return normal;
        case space_ch:
        case tab_ch:
            ctx.word[ctx.word_idx] = '\0';
            ctx.word_idx = 0;
            add_to_list(list, ctx.word);
            return normal;
        default:
            ctx.word[ctx.word_idx++] = ev;
            return normal;
    }
}

static enum state process_quote(enum event ev)
{
    switch (ev) {
        case quote_ch:
            return normal;
        case escape_ch:
            return escape;
        case nl_ch:
            fputs("Failed to parse a line: incorrect number of quotes.\n", stderr);
            free_list(list);
            return normal;
        case space_ch:
        case tab_ch:
        default:
            ctx.word[ctx.word_idx++] = ev;
            return quote;
    }
}

static enum state process_escape(enum event ev)
{
    ctx.word[ctx.word_idx++] = ev;
    return prev_st;
}
