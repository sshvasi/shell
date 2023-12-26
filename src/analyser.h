#ifndef ANALYSER_H
#define ANALYSER_H

#include "buffer.h"
#include "list.h"

enum state {
    state_normal,
    state_quote,
    state_escape
};

enum event {
    event_quote   = '"',
    event_escape  = '\\',
    event_space   = ' ',
    event_tab     = '\t',
    event_newline = '\n',
 /* default       = any character */
};

struct analyser {
    struct buffer *chars;
    struct list *tokens;
    struct {
        enum state curr;
        enum state prev;
    } *states;
    struct {
        enum event curr;
        enum event prev;
    } *events;
};

typedef enum state (*handler)(struct analyser *, enum event);

struct analyser *init_analyser(struct buffer *, struct list *);
void free_analyser(struct analyser *);
void analyse(struct analyser *, enum event);

#endif /* ANALYSER_H */
