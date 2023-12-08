#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#include "buffer.h"
#include "repl.h"

struct list *list;
struct buffer *buff;

enum event prev_event, curr_event;
enum state prev_state = state_normal, curr_state = state_normal;

static void handle(enum event ev);
static enum state handle_normal_state(enum event ev);
static enum state handle_quote_state(enum event ev);
static enum state handle_escape_state(enum event ev);

void init_repl()
{
    list = init_list();
    buff = init_buffer();

    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            handle(ch);
        }

        if (ch == '\n') {
            handle(ch);
            continue;
        }

        if (ch == EOF) {
            handle(ch);
            break;
        }
    }
}

static void handle(enum event ev)
{
    static const handler handlers[] = {
        handle_normal_state,
        handle_quote_state,
        handle_escape_state
    };

    prev_event = curr_event;
    curr_event = ev;

    enum state temp_state = curr_state;
    curr_state = handlers[curr_state](ev);
    prev_state = temp_state;

    if (prev_state != curr_state) {
        TRACE("[REPL] Transition [%d]->[%d].\n", prev_state, curr_state);
    }
}

static enum state handle_normal_state(enum event ev)
{
    switch (ev) {
        case event_quote:
            return state_quote;
        case event_escape:
            return state_escape;
        case event_newline:
            add_to_list(list, buff->store);
            empty_buffer(buff);
            print_list(list);
            empty_list(list);
            return state_normal;
        case event_space:
        case event_tab:
            add_to_list(list, buff->store);
            empty_buffer(buff);
            return state_normal;
        case EOF:
            free_buffer(buff);
            free_list(list);
            return state_normal;
        default:
            add_to_buffer(buff, ev);
            return state_normal;
    }
}

static enum state handle_quote_state(enum event ev)
{
    switch (ev) {
        case event_quote:
            return state_normal;
        case event_escape:
            return state_escape;
        case event_newline:
            fputs("Failed to parse a line: incorrect number of quotes.\n", stderr);
            empty_list(list);
            return state_normal;
        case EOF:
            free_buffer(buff);
            free_list(list);
            return state_normal;
        case event_space:
        case event_tab:
        default:
            add_to_buffer(buff, ev);
            return state_quote;
    }
}

static enum state handle_escape_state(enum event ev)
{
    switch (ev) {
    case EOF:
        free_buffer(buff);
        free_list(list);
        return state_normal;
    default:
        add_to_buffer(buff, ev);
        return prev_state;
    }
}
