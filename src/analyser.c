#include <stdio.h>
#include <stdlib.h>

#include "analyser.h"
#include "buffer.h"
#include "debug.h"
#include "executor.h"
#include "list.h"
#include "prompt.h"

static enum state handle_normal_state(struct analyser *an, enum event next_event);
static enum state handle_escape_state(struct analyser *an, enum event next_event);
static enum state handle_quote_state(struct analyser *an, enum event next_event);

struct analyser *init_analyser(struct buffer *chars, struct list *tokens)
{
    struct analyser *an;

    TRACE("[ANALYSER] Initilize.\n");

    an = malloc(sizeof(struct analyser));
    if (an == NULL) {
        perror("Failed to allocate memory for analyser.");
        exit(EXIT_FAILURE);
    }

    an->states = malloc(sizeof(enum state) * 2);
    if (an->states == NULL) {
        perror("Failed to allocate memory for analyser states.");
        free(an);
        exit(EXIT_FAILURE);
    }

    an->events = malloc(sizeof(enum event) * 2);
    if (an->events == NULL) {
        perror("Failed to allocate memory for analyser events.");
        free(an->states);
        free(an);
        exit(EXIT_FAILURE);
    }

    an->chars = chars;
    an->tokens = tokens;

    return an;
}

void free_analyser(struct analyser *an)
{
    TRACE("[ANALYSER] Free.\n");

    if (an == NULL) {
        return;
    }

    if (an->events != NULL) {
        free(an->events);
    }

    if (an->states != NULL) {
        free(an->states);
    }

    free_buffer(an->chars);
    free_list(an->tokens);

    free(an);
}

void analyse(struct analyser *an, enum event next_event)
{
    const static handler handlers[] = {
        handle_normal_state,
        handle_quote_state,
        handle_escape_state
    };

    an->events->prev = an->events->curr;
    an->events->curr = next_event;

    enum state temp_state = an->states->curr;
    an->states->curr = handlers[an->states->curr](an, next_event);
    an->states->prev = temp_state;
}

static enum state handle_normal_state(struct analyser *an, enum event next_event)
{
    switch (next_event) {
        case event_quote:
            return state_quote;
        case event_escape:
            return state_escape;
        case event_newline:
            add_to_list(an->tokens, an->chars);
            execute(an);
            empty_buffer(an->chars);
            empty_list(an->tokens);
            print_prompt(singleline_prompt);
            return state_normal;
        case event_space:
        case event_tab:
            add_to_list(an->tokens, an->chars);
            empty_buffer(an->chars);
            return state_normal;
        default:
            add_to_buffer(an->chars, next_event);
            return state_normal;
    }
}

static enum state handle_quote_state(struct analyser *an, enum event next_event)
{
    switch (next_event) {
        case event_quote:
            return state_normal;
        case event_escape:
            return state_escape;
        case event_newline:
            fputs("Failed to parse a line: incorrect number of quotes.\n", stderr);
            empty_buffer(an->chars);
            empty_list(an->tokens);
            print_prompt(singleline_prompt);
            return state_normal;
        case event_space:
        case event_tab:
        default:
            add_to_buffer(an->chars, next_event);
            return state_quote;
    }
}

static enum state handle_escape_state(struct analyser *an, enum event next_event)
{
    switch (next_event) {
    case event_newline:
        print_prompt(multiline_prompt);
        return an->states->prev;
    case event_quote:
    case event_space:
    case event_tab:
    case event_escape:
    default:
        add_to_buffer(an->chars, next_event);
        return an->states->prev;
    }
}

