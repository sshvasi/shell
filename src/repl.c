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

void init_repl()
{
    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        int ch;
        list = init_list();
        ctx.word_idx = 0;

        while ((ch = getchar()) != '\n' && ch != EOF) {
            process(ch);
        }

        if (ch == '\n') {
            process(ch);
            continue;
        }
        if (ch == EOF) {
            process(ch);
            break;
        }
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
        case EOF:
            free_list(list);
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
        case EOF:
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
    switch (ev) {
    case EOF:
        free_list(list);
        return normal;
    default:
        ctx.word[ctx.word_idx++] = ev;
        return prev_st;
    }
}
