#ifndef REPL_H
#define REPL_H

#include <stdio.h>

#include "buffer.h"
#include "list.h"

enum state {
    state_normal,
    state_quote,
    state_escape
};

enum event {
    event_quote = '"',
    event_escape = '\\',
    event_space = ' ',
    event_tab = '\t',
    event_newline = '\n',
    event_eof = EOF
    /* default = any character */
};

typedef enum state (*handler)(enum event, struct list*, struct buffer*);

void init_repl(struct list*, struct buffer*);

#endif /* REPL_H */
