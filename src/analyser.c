#include <stdio.h>
#include <stdlib.h>

#include "analyser.h"
#include "buffer.h"
#include "debug.h"
#include "executor.h"
#include "list.h"
#include "prompt.h"

enum event prev_event, curr_event;
enum state prev_state = state_normal, curr_state = state_normal;

static enum state handle_normal_state(enum event next_event,
                                      struct list *ls,
                                      struct buffer *buff);
static enum state handle_escape_state(enum event next_event,
                                      struct list *ls,
                                      struct buffer *buff);
static enum state handle_quote_state(enum event next_event,
                                     struct list *ls,
                                     struct buffer *buff);

void analyse(enum event next_event,
             struct list *ls,
             struct buffer *buff)
{
    static handler handlers[] = {
        handle_normal_state,
        handle_quote_state,
        handle_escape_state
    };

    prev_event = curr_event;
    curr_event = next_event;

    enum state temp_state = curr_state;
    curr_state = handlers[curr_state](next_event, ls, buff);
    prev_state = temp_state;

    /**
     * if (prev_state != curr_state) {
     *     TRACE("[REPL] Transition [%d]->[%d].\n", prev_state, curr_state);
     * }
    */
}

static enum state handle_normal_state(enum event next_event,
                                      struct list *ls,
                                      struct buffer *buff)
{
    switch (next_event) {
        case event_quote:
            return state_quote;
        case event_escape:
            return state_escape;
        case event_newline:
            add_to_list(ls, buff);
            empty_buffer(buff);
            execute(ls);
            empty_list(ls);
            print_prompt(singleline_prompt);
            return state_normal;
        case event_space:
        case event_tab:
            add_to_list(ls, buff);
            empty_buffer(buff);
            return state_normal;
        default:
            add_to_buffer(buff, next_event);
            return state_normal;
    }
}

static enum state handle_quote_state(enum event next_event,
                                     struct list *ls,
                                     struct buffer *buff)
{
    switch (next_event) {
        case event_quote:
            return state_normal;
        case event_escape:
            return state_escape;
        case event_newline:
            fputs("Failed to parse a line: incorrect number of quotes.\n", stderr);
            empty_buffer(buff);
            empty_list(ls);
            print_prompt(singleline_prompt);
            return state_normal;
        case event_space:
        case event_tab:
        default:
            add_to_buffer(buff, next_event);
            return state_quote;
    }
}

static enum state handle_escape_state(enum event next_event,
                                      struct list *ls,
                                      struct buffer *buff)
{
    switch (next_event) {
    case event_newline:
        print_prompt(multiline_prompt);
        return state_normal;
    default:
        add_to_buffer(buff, next_event);
        return prev_state;
    }
}

