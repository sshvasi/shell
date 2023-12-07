#ifndef REPL_H
#define REPL_H

#include "list.h"

#define BUFFER_SIZE 1024

struct context {
    char buffer[BUFFER_SIZE + 1];
    char word[BUFFER_SIZE + 1];

    int word_idx;
    int buff_idx;
};

enum state {
    normal,
    quote,
    escape
};

enum event {
    quote_ch = '"',
    escape_ch = '\\',
    space_ch = ' ',
    tab_ch = '\t',
    nl_ch = '\n'
    /* default = any character */
};

typedef enum state (*processor)(enum event);

void init_repl();

#endif /* REPL_H */
