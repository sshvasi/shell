#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#include "buffer.h"
#include "repl.h"

struct list *list;
struct buffer *buff;

enum event prev_ev, curr_ev;
enum state prev_st = normal, curr_st = normal;

static void process(enum event ev);
static enum state process_normal(enum event ev);
static enum state process_quote(enum event ev);
static enum state process_escape(enum event ev);

void init_repl()
{
    list = init_list();
    buff = init_buffer();

    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        int ch;
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

    if (prev_st != curr_st) {
        const char *event_names[] = {
            "normal",
            "in a quote",
            "escape sequence"
        };

        TRACE("[REPL] Transition [%s]->[%s].\n",
              event_names[prev_st],
              event_names[curr_st]);
    }
}

static enum state process_normal(enum event ev)
{
    switch (ev) {
        case quote_ch:
            return quote;
        case escape_ch:
            return escape;
        case nl_ch:
            add_to_list(list, buff->store);
            empty_buffer(buff);
            print_list(list);
            empty_list(list);
            return normal;
        case space_ch:
        case tab_ch:
            add_to_list(list, buff->store);
            empty_buffer(buff);
            return normal;
        case EOF:
            free_buffer(buff);
            free_list(list);
            return normal;
        default:
            add_to_buffer(buff, ev);
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
            empty_list(list);
            return normal;
        case EOF:
            free_buffer(buff);
            free_list(list);
            return normal;
        case space_ch:
        case tab_ch:
        default:
            add_to_buffer(buff, ev);
            return quote;
    }
}

static enum state process_escape(enum event ev)
{
    switch (ev) {
    case EOF:
        free_buffer(buff);
        free_list(list);
        return normal;
    default:
        add_to_buffer(buff, ev);
        return prev_st;
    }
}
